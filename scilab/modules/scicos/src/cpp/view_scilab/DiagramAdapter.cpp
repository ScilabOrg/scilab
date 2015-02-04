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
#include <algorithm>
#include <iterator>

#include "internal.hxx"
#include "double.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
#include "DiagramAdapter.hxx"
#include "Adapters.hxx"
#include "ParamsAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"
#include "TextAdapter.hxx"
#include "model/BaseObject.hxx"
#include "recursive_cloning.hxx"

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

const std::wstring Deleted (L"Deleted");
const std::wstring TextSharedTypeStr (L"Text");

struct props
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        ParamsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        return localAdaptor.getAsTList(new types::TList(), controller);
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ParamsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        return localAdaptor.setAsTList(v, controller);
    }
};

struct objs
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& /*controller*/)
    {
        // FIXME: reconstruct the list of objects accordingly to the CHILDREN content to manage xcos modification
        return adaptor.getListObjects();
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // Decode the list and set all children of the Diagram
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();
        types::List* list = v->getAs<types::List>();

        std::vector<ScicosID> oldDiagramChildren;
        controller.getObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, oldDiagramChildren);

        /*
         * First pass on objects :
         *  - store IDs if they exists and are valid ; 0ll otherwise
         *  - store all the links to update link connections later
         *  - store all the valid mlist content ('Text' content)
         *  - store all the deleted mlist content ('Deleted' content)
         */

        std::vector<ScicosID> diagramChildren;
        diagramChildren.reserve(list->getSize());
        std::vector<kind_t> diagramChildrenKind;
        diagramChildrenKind.reserve(list->getSize());

        std::vector<LinkAdapter*> links;
        std::vector<int> textAsMListIndices;
        std::vector<int> deletedAsMListIndices;

        for (int i = 0; i < list->getSize(); ++i)
        {
            if (list->get(i)->getType() == types::InternalType::ScilabUserType)
            {
                const Adapters::adapters_index_t adapter_index = Adapters::instance().lookup_by_typename(list->get(i)->getShortTypeStr());
                ScicosID id;
                kind_t kind;
                switch (adapter_index)
                {
                    case Adapters::BLOCK_ADAPTER:
                    {
                        BlockAdapter* modelElement = list->get(i)->getAs<BlockAdapter>();
                        id = modelElement->getAdaptee()->id();
                        kind = modelElement->getAdaptee()->kind();
                        break;
                    }
                    case Adapters::LINK_ADAPTER:
                    {
                        LinkAdapter* modelElement = list->get(i)->getAs<LinkAdapter>();
                        id = modelElement->getAdaptee()->id();
                        kind = modelElement->getAdaptee()->kind();

                        // Do the linking in the next loop, in case the Link points to a Block that has not been added yet
                        links.push_back(modelElement);
                        break;
                    }
                    case Adapters::TEXT_ADAPTER:
                    {
                        TextAdapter* modelElement = list->get(i)->getAs<TextAdapter>();
                        id = modelElement->getAdaptee()->id();
                        kind = modelElement->getAdaptee()->kind();
                        break;
                    }
                    default:
                        return false;
                }

                diagramChildren.push_back(id);
                diagramChildrenKind.push_back(kind);
            }
            else if (list->get(i)->getType() == types::InternalType::ScilabMList)
            {
                // Allow to pass mlists to 'objs', representing 'Deleted' or 'Text' objects
                types::MList* modelElement = list->get(i)->getAs<types::MList>();
                types::String* header = modelElement->getFieldNames();

                if (header->get(0) == Deleted)
                {
                    diagramChildren.push_back(0ll);
                    diagramChildrenKind.push_back(ANNOTATION);
                }
                else if (header->get(0) == TextSharedTypeStr)
                {
                    textAsMListIndices.push_back(i);
                    // will be filled later
                    diagramChildren.push_back(0ll);
                    diagramChildrenKind.push_back(ANNOTATION);
                }
                else
                {
                    return false;
                }
            }
            else if (list->get(i)->getType() == types::InternalType::ScilabList)
            {
                // Allow to pass empty lists to 'objs', representing deleted Blocks
                types::List* modelElement = list->get(i)->getAs<types::List>();
                if (modelElement->getSize() != 0)
                {
                    return false;
                }

                deletedAsMListIndices.push_back(i);
                // Mark deleted objects with value '0'
                diagramChildren.push_back(0ll);
                diagramChildrenKind.push_back(ANNOTATION);
            }
            else
            {
                return false;
            }
        }

        /*
         * Create all Annotation, decoding mlist content
         */
        for (const auto index : textAsMListIndices)
        {
            ScicosID localAdaptee = controller.createObject(ANNOTATION);
            TextAdapter* localAdaptor = new TextAdapter(controller, controller.getObject<model::Annotation>(localAdaptee));
            if (!localAdaptor->setAsTList(list->get(index), controller))
            {
                // do not return there ; the annotation will be empty
            }

            diagramChildren[index] = localAdaptee;
            list->set(index, localAdaptor); // also update the list content
        }

        /*
         * Recreate 'Deleted' mlist if needed
         */
        for (const auto index : deletedAsMListIndices)
        {
            types::MList* deleted = new types::MList();
            deleted->set(0, new types::String(Deleted.data()));

            list->set(index, deleted); // also update the list content
        }

        /*
         * Set the parent diagram of all the blocks
         */
        {
            std::vector<ScicosID> sortedDiagramChildren = diagramChildren;
            std::sort(sortedDiagramChildren.begin(), sortedDiagramChildren.end());
            std::sort(oldDiagramChildren.begin(), oldDiagramChildren.end());

            std::vector<ScicosID> oldDiff;
            std::set_difference(oldDiagramChildren.begin(), oldDiagramChildren.end(), sortedDiagramChildren.begin(), sortedDiagramChildren.end(), std::inserter(oldDiff, oldDiff.begin()));
            for (const ScicosID id : oldDiff)
            {
                if (id != 0)
                {
                    auto o = controller.getObject(id);
                    controller.setObjectProperty(o->id(), o->kind(), PARENT_DIAGRAM, 0ll);
                }
            }

            std::vector<ScicosID> newDiff;
            std::set_difference(sortedDiagramChildren.begin(), sortedDiagramChildren.end(), oldDiagramChildren.begin(), oldDiagramChildren.end(), std::inserter(newDiff, newDiff.begin()));
            for (const ScicosID id : newDiff)
            {
                if (id != 0)
                {
                    auto o = controller.getObject(id);
                    controller.setObjectProperty(o->id(), o->kind(), PARENT_DIAGRAM, adaptee->id());

                    // reference it
                    controller.referenceObject(o);
                }
            }
        }

        /*
         * Store the children to the model and to the local adaptor
         */
        controller.setObjectProperty(adaptee->id(), DIAGRAM, CHILDREN, diagramChildren);
        adaptor.setListObjects(v);

        /*
         * Re-sync the partial link information
         */
        std::vector<link_t> from_content (links.size());
        std::vector<link_t> to_content (links.size());
        // Do the linking at model-level
        for (int i = 0; i < static_cast<int>(links.size()); ++i)
        {
            // Trigger 'from' and 'to' properties
            from_content[i] = links[i]->getFrom();
            links[i]->setFromInModel(from_content[i], controller);

            to_content[i] = links[i]->getTo();
            links[i]->setToInModel(to_content[i], controller);
        }
        adaptor.setFrom(from_content);
        adaptor.setTo(to_content);

        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

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

            model::Diagram* adaptee = adaptor.getAdaptee();

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

            model::Diagram* adaptee = adaptor.getAdaptee();

            std::string version;
            controller.setObjectProperty(adaptee->id(), DIAGRAM, VERSION_NUMBER, version);
            return true;
        }

        return false;
    }
};

struct contrib
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getContribContent();
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& /*controller*/)
    {
        adaptor.setContribContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(const Controller& c, org_scilab_modules_scicos::model::Diagram* adaptee) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(c, adaptee),
    list_objects(nullptr),
    from_vec(),
    to_vec(),
    contrib_content(nullptr)
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::fields.reserve(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
    }

    setListObjects(new types::List());
    setContribContent(new types::List());
}

DiagramAdapter::DiagramAdapter(const DiagramAdapter& adapter) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(adapter),
    list_objects(nullptr),
    from_vec(adapter.from_vec),
    to_vec(adapter.to_vec),
    contrib_content(nullptr)
{
    setListObjects(adapter.getListObjects()->clone());
    setContribContent(adapter.getContribContent()->clone());
}

DiagramAdapter::~DiagramAdapter()
{
    list_objects->DecreaseRef();
    list_objects->killMe();

    contrib_content->DecreaseRef();
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
    return contrib_content;
}

void DiagramAdapter::setContribContent(types::InternalType* v)
{
    types::InternalType* temp = contrib_content;

    // do not check if v is nullptr on purpose ; it *should* not
    v->IncreaseRef();
    contrib_content = v;

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }


}

types::InternalType* DiagramAdapter::getListObjects() const
{
    return list_objects;
}

void DiagramAdapter::setListObjects(types::InternalType* v)
{
    types::InternalType* temp = list_objects;

    // do not check if v is nullptr on purpose ; it *should* not
    v->IncreaseRef();
    list_objects = v;

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }
}

std::vector<link_t> DiagramAdapter::getFrom() const
{
    return from_vec;
}

void DiagramAdapter::setFrom(const std::vector<link_t>& from)
{
    from_vec = from;
}

std::vector<link_t> DiagramAdapter::getTo() const
{
    return to_vec;
}

void DiagramAdapter::setTo(const std::vector<link_t>& to)
{
    to_vec = to;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
