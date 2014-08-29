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
#include "model/BaseObject.hxx"
#include "DiagramAdapter.hxx"
#include "ParamsAdapter.hxx"
#include "TextAdapter.hxx"
#include "BlockAdapter.hxx"
#include "LinkAdapter.hxx"

extern "C" {
#include "version.h"
}

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
namespace
{

static const std::wstring Annotation = L"Text";
static const std::wstring Block = L"Block";
static const std::wstring Link = L"Link";

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
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> children;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CHILDREN, children);

        types::List* o = new types::List();

        for (size_t i = 0; i < children.size(); ++i)
        {
            model::BaseObject* item = Controller().getObject(children[i]);
            switch (item->kind())
            {
                case ANNOTATION:
                {
                    model::Annotation* annotation = static_cast<model::Annotation*>(item);
                    TextAdapter localAdaptor = TextAdapter(annotation);
                    o->set(i, localAdaptor.getAsTList(new types::MList(), controller));
                    break;
                }
                case BLOCK:
                {
                    model::Block* block = static_cast<model::Block*>(item);
                    BlockAdapter localAdaptor = BlockAdapter(block);
                    o->set(i, localAdaptor.getAsTList(new types::MList(), controller));
                    break;
                }
                case LINK:
                {
                    model::Link* link = static_cast<model::Link*>(item);
                    LinkAdapter localAdaptor = LinkAdapter(link);
                    o->set(i, localAdaptor.getAsTList(new types::MList(), controller));
                    break;
                }
                default:
                    return 0;
            }
        }
        return o;
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabList)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        types::List* list = v->getAs<types::List>();
        for (int i = 0; i < list->getSize(); ++i)
        {
            if (list->get(i)->getType() != types::InternalType::ScilabMList)
            {
                return false;
            }
            types::MList* modelElement = list->get(i)->getAs<types::MList>();

            types::String* fieldNames = modelElement->getFieldNames();
            std::wstring modelElementType = modelElement->getTypeStr();
            if (modelElementType == Annotation)
            {

            }
            else if (modelElementType == Block)
            {
                if (fieldNames->getSize() != 19)
                {
                    return false;
                }
                ScicosID id = controller.createObject(BLOCK);
                BlockAdapter* localAdaptor = new BlockAdapter(static_cast<model::Block*>(controller.getObject(id)));

                for (int j = 1; j < modelElement->getSize(); ++j)
                {
                    std::wstring name = fieldNames->get(j);
                    // FIXME: modelElement may another type than Double
                    types::Double* fieldValue = modelElement->getAs<types::Double>();
                    if (!localAdaptor->setProperty(name, fieldValue, controller))
                    {
                        return false;
                    }
                    // FIXME: std::vector<ScicosID> children; before the loop
                    // children[i] = id;
                    // controller.setObjectProperty(adaptee->kind(), adaptee->id(), CHILDREN, children) after the loop
                }
            }
            else if (modelElementType == Link)
            {
            }
            else
            {
                return false;
            }
        }
        return true;
    }
};

struct version
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        std::stringstream str;
        str << SCI_VERSION_MAJOR << '.' << SCI_VERSION_MINOR << '.' << SCI_VERSION_MAINTENANCE;

        return new types::String(str.str().c_str());
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        //FIXME: handle version upgrade of the whole model
        return true;
    }
};

struct contrib
{

    static types::InternalType* get(const DiagramAdapter& adaptor, const Controller& controller)
    {
        return 0;
    }

    static bool set(DiagramAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return false;
    }
};

} /* namespace */

template<> property<DiagramAdapter>::props_t property<DiagramAdapter>::fields = property<DiagramAdapter>::props_t();

DiagramAdapter::DiagramAdapter(const DiagramAdapter& o) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(o), contrib(o.contrib) {}

DiagramAdapter::DiagramAdapter(org_scilab_modules_scicos::model::Diagram* o) :
    BaseAdapter<DiagramAdapter, org_scilab_modules_scicos::model::Diagram>(o), contrib(0)
{
    if (property<DiagramAdapter>::properties_have_not_been_set())
    {
        property<DiagramAdapter>::fields.reserve(4);
        property<DiagramAdapter>::add_property(L"props", &props::get, &props::set);
        property<DiagramAdapter>::add_property(L"objs", &objs::get, &objs::set);
        property<DiagramAdapter>::add_property(L"version", &version::get, &version::set);
        property<DiagramAdapter>::add_property(L"contrib", &contrib::get, &contrib::set);
    }
}

DiagramAdapter::~DiagramAdapter()
{
}

std::wstring DiagramAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring DiagramAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* DiagramAdapter::getContrib() const
{
    return contrib;
}

void DiagramAdapter::setContrib(types::InternalType* contrib)
{
    this->contrib = contrib;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
