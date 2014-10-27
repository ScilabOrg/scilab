/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string>
#include <vector>
#include <sstream>

#include "internal.hxx"
#include "double.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "utilities.hxx"
#include "Controller.hxx"
#include "DiagramAdapter.hxx"
#include "Adapters.hxx"
#include "ParamsAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"
#include "TextAdapter.hxx"
#include "model/BaseObject.hxx"

extern "C" {
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

struct props
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        ParamsAdapter localAdaptor = ParamsAdapter(adaptor.getAdaptee());
        return localAdaptor.getAsTList(new types::TList(), controller);
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ParamsAdapter localAdaptor = ParamsAdapter(adaptor.getAdaptee());
        return localAdaptor.setAsTList(v, controller);
    }
};

struct objs
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        // Get all children of the Diagram and return them as a list
        model::Diagram* adaptee = adaptor.getAdaptee().get();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, children);

        types::List* o = new types::List();

        std::vector<BlockAdapter*> blockAdapters = adaptor.getBlocks();
        std::vector<LinkAdapter*> linkAdapters = adaptor.getLinks();
        std::vector<TextAdapter*> textAdapters = adaptor.getTexts();
        int block_index = 0;
        int link_index = 0;
        int text_index = 0;
        Controller newController = Controller();
        for (int i = 0; i < static_cast<int>(children.size()); ++i)
        {
            model::BaseObject* item = newController.getObject(children[i]).get();
            switch (item->kind())
            {
                case ANNOTATION:
                {
                    o->set(i, textAdapters[text_index]);
                    text_index++;
                    continue;
                }
                case BLOCK:
                {
                    o->set(i, blockAdapters[block_index]);
                    block_index++;
                    continue;
                }
                case LINK:
                {
                    o->set(i, linkAdapters[link_index]);
                    link_index++;
                    continue;
                }
                default:
                    return 0;
            }
        }
        return o;
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // Decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee().get();

        types::List* list = v->getAs<types::List>();

        // Clear the children list before the loop to reset the diagram children
        // and clear the old objects information
        std::vector<ScicosID> diagramChildren;
        controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
        adaptor.clearBlocks();
        adaptor.clearLinks();
        adaptor.clearTexts();
        std::vector<LinkAdapter*> linkListView;
        for (int i = 0; i < list->getSize(); ++i)
        {
            if (list->get(i)->getType() != types::InternalType::ScilabUserType)
            {
                return false;
            }

            // Find the type of the input object through Adapters' mapping.
            const Adapters::adapters_index_t adapter_index = Adapters::instance().lookup_by_typename(list->get(i)->getShortTypeStr());

            // Then, each adapter gets linked to the diagram through its adaptee (PARENT_DIAGRAM)
            // and the diagram's adaptee lists its adaptees (CHILDREN).
            ScicosID id;
            switch (adapter_index)
            {
                case Adapters::BLOCK_ADAPTER:
                {
                    BlockAdapter* modelElement = list->get(i)->getAs<BlockAdapter>();
                    model::Block* subAdaptee = modelElement->getAdaptee().get();

                    id = subAdaptee->id();
                    adaptor.addBlock(modelElement);

                    controller.setObjectProperty(id, BLOCK, PARENT_DIAGRAM, adaptee->id());
                    controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    diagramChildren.push_back(id);
                    controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    break;
                }
                case Adapters::LINK_ADAPTER:
                {
                    LinkAdapter* modelElement = list->get(i)->getAs<LinkAdapter>();
                    model::Link* subAdaptee = modelElement->getAdaptee().get();

                    id = subAdaptee->id();
                    adaptor.addLink(modelElement);

                    controller.setObjectProperty(id, LINK, PARENT_DIAGRAM, adaptee->id());

                    // Hold Links information, to try the linking at model-level once all the elements have been added to the Diagram
                    linkListView.push_back(modelElement);

                    controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    diagramChildren.push_back(id);
                    controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    break;
                }
                case Adapters::TEXT_ADAPTER:
                {
                    TextAdapter* modelElement = list->get(i)->getAs<TextAdapter>();
                    model::Annotation* subAdaptee = modelElement->getAdaptee().get();

                    id = subAdaptee->id();
                    adaptor.addText(modelElement);

                    controller.setObjectProperty(id, ANNOTATION, PARENT_DIAGRAM, adaptee->id());
                    controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    diagramChildren.push_back(id);
                    controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
                    break;
                }
                default:
                    return false;
            }
        }

        // Do the linking at model-level
        for (std::vector<LinkAdapter*>::iterator it = linkListView.begin(); it != linkListView.end(); ++it)
        {
            // Trigger each Link's 'from' and 'to' properties
            (*it)->setFrom((*it)->getFrom(), controller);
            (*it)->setTo((*it)->getTo(), controller);
        }

        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee().get();

        std::string version;
        controller.getObjectProperty(adaptee->id(), DIAGRAM, VERSION_NUMBER, version);

        return new types::String(version.data());
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee().get();

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string version (c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee->id(), DIAGRAM, VERSION_NUMBER, version);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee().get();

            std::string version;
            controller.setObjectProperty(adaptee->id(), DIAGRAM, VERSION_NUMBER, version);
            return true;
        }

        return false;
    }
};

struct contrib
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        return adaptor.getContribContent();
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // silent unused parameter warnings
        (void) v;
        (void) controller;

        adaptor.setContribContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Diagram> adaptee) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(adaptee)
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::fields.reserve(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
    }

    contrib_content = new types::List();
}

DiagramAdapter::DiagramAdapter(const DiagramAdapter& adapter) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(adapter)
{
    // When cloning a DiagramAdapter, clone its objects information as well
    for (std::vector<BlockAdapter*>::const_iterator it = adapter.blockAdapters.begin(); it != adapter.blockAdapters.end(); ++it)
    {
        blockAdapters.push_back(new BlockAdapter(**it));
    }
    for (std::vector<LinkAdapter*>::const_iterator it = adapter.linkAdapters.begin(); it != adapter.linkAdapters.end(); ++it)
    {
        linkAdapters.push_back(new LinkAdapter(**it));
    }
    for (std::vector<TextAdapter*>::const_iterator it = adapter.textAdapters.begin(); it != adapter.textAdapters.end(); ++it)
    {
        textAdapters.push_back(new TextAdapter(**it));
    }

    contrib_content = adapter.contrib_content->clone();
}

DiagramAdapter::~DiagramAdapter()
{
    // Kill all the elements of the diagram
    for (std::vector<BlockAdapter*>::iterator it = blockAdapters.begin(); it != blockAdapters.end(); ++it)
    {
        (*it)->killMe();
    }
    for (std::vector<LinkAdapter*>::iterator it = linkAdapters.begin(); it != linkAdapters.end(); ++it)
    {
        (*it)->killMe();
    }
    for (std::vector<TextAdapter*>::iterator it = textAdapters.begin(); it != textAdapters.end(); ++it)
    {
        (*it)->killMe();
    }

    contrib_content->killMe();
}

std::wstring DiagramAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring DiagramAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* DiagramAdapter::getContribContent() const
{
    return contrib_content->clone();
}

void DiagramAdapter::setContribContent(types::InternalType* v)
{
    contrib_content->killMe();
    contrib_content = v->clone();
}

std::vector<BlockAdapter*> DiagramAdapter::getBlocks() const
{
    std::vector<BlockAdapter*> blocks (blockAdapters.size());

    for (int i = 0; i < static_cast<int>(blocks.size()); ++i)
    {
        blocks[i] = new BlockAdapter(*(blockAdapters[i]));
    }
    return blocks;
}

void DiagramAdapter::addBlock(const BlockAdapter* newBlock)
{
    blockAdapters.push_back(new BlockAdapter(*newBlock));
}

void DiagramAdapter::clearBlocks()
{
    for (std::vector<BlockAdapter*>::iterator it = blockAdapters.begin(); it != blockAdapters.end(); ++it)
    {
        (*it)->killMe();
    }
}

std::vector<LinkAdapter*> DiagramAdapter::getLinks() const
{
    std::vector<LinkAdapter*> links (linkAdapters.size());

    for (int i = 0; i < static_cast<int>(links.size()); ++i)
    {
        links[i] = new LinkAdapter(*(linkAdapters[i]));
    }
    return links;
}

void DiagramAdapter::addLink(const LinkAdapter* newLink)
{
    linkAdapters.push_back(new LinkAdapter(*newLink));
}

void DiagramAdapter::clearLinks()
{
    for (std::vector<LinkAdapter*>::iterator it = linkAdapters.begin(); it != linkAdapters.end(); ++it)
    {
        (*it)->killMe();
    }
}

std::vector<TextAdapter*> DiagramAdapter::getTexts() const
{
    std::vector<TextAdapter*> texts (textAdapters.size());

    for (int i = 0; i < static_cast<int>(texts.size()); ++i)
    {
        texts[i] = new TextAdapter(*(textAdapters[i]));
    }
    return texts;
}

void DiagramAdapter::addText(const TextAdapter* newText)
{
    textAdapters.push_back(new TextAdapter(*newText));
}

void DiagramAdapter::clearTexts()
{
    for (std::vector<TextAdapter*>::iterator it = textAdapters.begin(); it != textAdapters.end(); ++it)
    {
        (*it)->killMe();
    }
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
