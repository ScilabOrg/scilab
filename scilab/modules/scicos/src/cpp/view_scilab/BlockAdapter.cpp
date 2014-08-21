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
#include <algorithm>

#include "internal.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "Controller.hxx"
#include "model/Block.hxx"
#include "BlockAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "ModelAdapter.hxx"

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

struct graphics
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        int i = 1;
        GraphicsAdapter adaptee = GraphicsAdapter(adaptor.getAdaptee());

        types::MList* o = new types::MList();
        types::String* MListFields = new types::String(1, 19);
        types::InternalType* currentField;

        typename property<BlockAdapter>::props_t BlockProperties = property<BlockAdapter>::fields;
        typename property<GraphicsAdapter>::props_t GraphicsProperties = property<GraphicsAdapter>::fields;
        std::sort(BlockProperties.begin(), BlockProperties.end(), property<BlockAdapter>::original_index_cmp);
        std::sort(GraphicsProperties.begin(), GraphicsProperties.end(), property<GraphicsAdapter>::original_index_cmp);

        MListFields->set(0, BlockProperties[0].name.data());
        o->set(0, MListFields);

        for (typename property<GraphicsAdapter>::props_t_it it = GraphicsProperties.begin(); it != GraphicsProperties.end(); ++it, ++i)
        {
            //currentField = adaptee.getProperty(it->name.data(), controller);
            MListFields->set(i, it->name.data());
            o->set(i, currentField);
        }

        o->set(0, MListFields);
        return o;
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabMList
                && v->getShortTypeStr() == GraphicsAdapter::getSharedTypeStr())
        {
            GraphicsAdapter* graphics = new GraphicsAdapter(adaptor.getAdaptee());

            types::MList* current = v->getAs<types::MList>();
            if (current->getSize() != 19)
            {
                return false;
            }

            typename property<GraphicsAdapter>::props_t GraphicsProperties = property<GraphicsAdapter>::fields;
            std::sort(GraphicsProperties.begin(), GraphicsProperties.end(), property<GraphicsAdapter>::original_index_cmp);
            types::InternalType* currentField;

            for (typename property<GraphicsAdapter>::props_t_it it = GraphicsProperties.begin(); it != GraphicsProperties.end(); ++it)
            {
                if ((currentField = current->getField(it->name.data())) == NULL)
                {
                    return false;
                }
                if (!graphics->setProperty(it->name.data(), currentField, controller))
                {
                    return false;
                }
            }

            adaptor.setAdaptee(graphics->getAdaptee());
            return true;
        }
        return false;
    }
};

struct model
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        int i = 1;
        ModelAdapter adaptee = ModelAdapter(adaptor.getAdaptee());

        types::MList* o = new types::MList();
        types::String* MListFields = new types::String(1, 24);
        types::InternalType* currentField;

        typename property<BlockAdapter>::props_t BlockProperties = property<BlockAdapter>::fields;
        typename property<ModelAdapter>::props_t ModelProperties = property<ModelAdapter>::fields;
        std::sort(BlockProperties.begin(), BlockProperties.end(), property<BlockAdapter>::original_index_cmp);
        std::sort(ModelProperties.begin(), ModelProperties.end(), property<ModelAdapter>::original_index_cmp);

        MListFields->set(0, BlockProperties[1].name.data());
        o->set(0, MListFields);

        for (typename property<ModelAdapter>::props_t_it it = ModelProperties.begin(); it != ModelProperties.end(); ++it, ++i)
        {
            //currentField = adaptee.getProperty(it->name.data(), controller);
            MListFields->set(i, it->name.data());
            o->set(i, currentField);
        }

        o->set(0, MListFields);
        return o;
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabMList
                && v->getShortTypeStr() == ModelAdapter::getSharedTypeStr())
        {
            ModelAdapter* model = new ModelAdapter(adaptor.getAdaptee());

            types::MList* current = v->getAs<types::MList>();
            if (current->getSize() != 24)
            {
                return false;
            }

            typename property<ModelAdapter>::props_t ModelProperties = property<ModelAdapter>::fields;
            std::sort(ModelProperties.begin(), ModelProperties.end(), property<ModelAdapter>::original_index_cmp);
            types::InternalType* currentField;

            for (typename property<ModelAdapter>::props_t_it it = ModelProperties.begin(); it != ModelProperties.end(); ++it)
            {
                if ((currentField = current->getField(it->name.data())) == NULL)
                {
                    return false;
                }
                if (!model->setProperty(it->name.data(), currentField, controller))
                {
                    return false;
                }
            }

            adaptor.setAdaptee(model->getAdaptee());
            return true;
        }
        return false;
    }
};

struct gui
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        std::string interface;
        org_scilab_modules_scicos::model::Block* adaptee = adaptor.getAdaptee();
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), INTERFACE_FUNCTION, interface);

        return new types::String(interface.data());
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (current->getRows() != 1 || current->getCols() != 1)
        {
            return false;
        }

        wchar_t* w_name = current->get(0);
        char* name = wide_string_to_UTF8(w_name);
        std::string stName(name);
        FREE(name);

        org_scilab_modules_scicos::model::Block* adaptee = adaptor.getAdaptee();
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), INTERFACE_FUNCTION, stName);
        return true;
    }
};

struct doc
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        return adaptor.getDocContent();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        adaptor.setDocContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<BlockAdapter>::props_t property<BlockAdapter>::fields = property<BlockAdapter>::props_t();

BlockAdapter::BlockAdapter(const BlockAdapter& o) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(o), doc_content(o.doc_content->clone()) {}

BlockAdapter::BlockAdapter(org_scilab_modules_scicos::model::Block* o) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(o)
{
    if (property<BlockAdapter>::properties_has_not_been_set())
    {
        property<BlockAdapter>::fields.reserve(4);
        property<BlockAdapter>::add_property(L"graphics", &graphics::get, &graphics::set);
        property<BlockAdapter>::add_property(L"model", &model::get, &model::set);
        property<BlockAdapter>::add_property(L"gui", &gui::get, &gui::set);
        property<BlockAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }

    doc_content = new types::List();
}

BlockAdapter::~BlockAdapter()
{
    delete doc_content;
}

std::wstring BlockAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring BlockAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* BlockAdapter::getDocContent() const
{
    return doc_content;
}

void BlockAdapter::setDocContent(types::InternalType* v)
{
    delete doc_content;
    doc_content = v->clone();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
