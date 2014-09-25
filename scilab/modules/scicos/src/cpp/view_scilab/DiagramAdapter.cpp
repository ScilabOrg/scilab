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
#include "TextAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"
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
        ParamsAdapter localAdaptor = ParamsAdapter(false, adaptor.getAdaptee());
        return localAdaptor.getAsTList(new types::TList(), controller);
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ParamsAdapter localAdaptor = ParamsAdapter(false, adaptor.getAdaptee());
        return localAdaptor.setAsTList(v, controller);
    }
};

struct objs
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        // FIXME: get all children of the Diagram and return them as a list
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);

        types::List* o = new types::List();

        for (int i = 0; i < static_cast<int>(children.size()); ++i)
        {
            model::BaseObject* item = Controller().getObject(children[i]);
            switch (item->kind())
            {
                case ANNOTATION:
                {
                    model::Annotation* annotation = static_cast<model::Annotation*>(item);
                    TextAdapter* localAdaptor = new TextAdapter(false, annotation);
                    o->set(i, localAdaptor);
                    continue;
                }
                case BLOCK:
                {
                    model::Block* block = static_cast<model::Block*>(item);
                    BlockAdapter* localAdaptor = new BlockAdapter(false, block);
                    o->set(i, localAdaptor);
                    continue;
                }
                case LINK:
                {
                    model::Link* link = static_cast<model::Link*>(item);
                    LinkAdapter* localAdaptor = new LinkAdapter(false, link);
                    o->set(i, localAdaptor);
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
        // FIXME implement, decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        types::List* list = v->getAs<types::List>();
        std::vector<ScicosID> diagramChildren (list->getSize());
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
                    model::Block* subAdaptee = modelElement->getAdaptee();

                    id = subAdaptee->id();

                    ScicosID oldID;
                    controller.getObjectProperty(id, subAdaptee->kind(), PARENT_DIAGRAM, oldID);
                    if (oldID != 0)
                    {
                        // if the object has already been added to a diagram ; clone it
                        id = controller.cloneObject(id);
                    }

                    controller.setObjectProperty(id, subAdaptee->kind(), PARENT_DIAGRAM, adaptee->id());
                    break;
                }
                case Adapters::LINK_ADAPTER:
                {
                    LinkAdapter* modelElement = list->get(i)->getAs<LinkAdapter>();
                    model::Link* subAdaptee = modelElement->getAdaptee();

                    controller.setObjectProperty(subAdaptee->id(), subAdaptee->kind(), PARENT_DIAGRAM, adaptee->id());
                    id = subAdaptee->id();
                    break;
                }
                case Adapters::TEXT_ADAPTER:
                {
                    TextAdapter* modelElement = list->get(i)->getAs<TextAdapter>();
                    model::Annotation* subAdaptee = modelElement->getAdaptee();

                    controller.setObjectProperty(subAdaptee->id(), subAdaptee->kind(), PARENT_DIAGRAM, adaptee->id());
                    id = subAdaptee->id();
                    break;
                }
                default:
                    return false;
            }

            diagramChildren[i] = id;
        }

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, diagramChildren);
        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::string version;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);

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

            model::Diagram* adaptee = adaptor.getAdaptee();

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string version (c_str);
            FREE(c_str);

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getSize() != 0)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee();

            std::string version;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), VERSION_NUMBER, version);
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

DiagramAdapter::DiagramAdapter(bool ownAdaptee, org_scilab_modules_scicos::model::Diagram* adaptee) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(ownAdaptee, adaptee)
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

DiagramAdapter::~DiagramAdapter()
{
    delete contrib_content;
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
    delete contrib_content;
    contrib_content = v->clone();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
