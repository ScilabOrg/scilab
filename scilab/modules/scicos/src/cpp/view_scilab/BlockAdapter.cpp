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
        // silent unused parameter warnings
        (void) controller;

        return adaptor.getGraphics();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        GraphicsAdapter* newGraphics = new GraphicsAdapter(adaptor.getAdaptee());
        if (!newGraphics->setAsTList(v, controller))
        {
            return false;
        }

        adaptor.setGraphics(newGraphics);
        return true;
    }
};

struct model
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        return adaptor.getModel();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ModelAdapter* newModel = new ModelAdapter(adaptor.getAdaptee());
        if (!newModel->setAsTList(v, controller))
        {
            return false;
        }

        adaptor.setModel(newModel);
        return true;
    }
};

struct gui
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        std::string Interface;
        ScicosID adaptee = adaptor.getAdaptee()->id();
        controller.getObjectProperty(adaptee, BLOCK, INTERFACE_FUNCTION, Interface);

        return new types::String(Interface.data());
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

        ScicosID adaptee = adaptor.getAdaptee()->id();
        controller.setObjectProperty(adaptee, BLOCK, INTERFACE_FUNCTION, stName);
        return true;
    }
};

struct doc
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        return adaptor.getDocContent();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        adaptor.setDocContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<BlockAdapter>::props_t property<BlockAdapter>::fields = property<BlockAdapter>::props_t();

BlockAdapter::BlockAdapter(std::shared_ptr<org_scilab_modules_scicos::model::Block> adaptee) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adaptee)
{
    if (property<BlockAdapter>::properties_have_not_been_set())
    {
        property<BlockAdapter>::fields.reserve(4);
        property<BlockAdapter>::add_property(L"graphics", &graphics::get, &graphics::set);
        property<BlockAdapter>::add_property(L"model", &model::get, &model::set);
        property<BlockAdapter>::add_property(L"gui", &gui::get, &gui::set);
        property<BlockAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }

    graphicsAdapter = new GraphicsAdapter(this->getAdaptee());
    modelAdapter = new ModelAdapter(this->getAdaptee());

    doc_content = new types::List();
}

BlockAdapter::BlockAdapter(const BlockAdapter& adapter) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adapter)
{
    graphicsAdapter = new GraphicsAdapter(*(adapter.graphicsAdapter));
    modelAdapter = new ModelAdapter(*(adapter.modelAdapter));

    doc_content = adapter.doc_content->clone();
}

BlockAdapter::~BlockAdapter()
{
    graphicsAdapter->killMe();
    modelAdapter->killMe();

    doc_content->killMe();
}

std::wstring BlockAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring BlockAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

GraphicsAdapter* BlockAdapter::getGraphics() const
{
    return new GraphicsAdapter(*graphicsAdapter);
}

void BlockAdapter::setGraphics(GraphicsAdapter* newGraphics)
{
    graphicsAdapter->killMe();
    graphicsAdapter = new GraphicsAdapter(*newGraphics);
}

ModelAdapter* BlockAdapter::getModel() const
{
    return new ModelAdapter(*modelAdapter);
}

void BlockAdapter::setModel(ModelAdapter* newModel)
{
    modelAdapter->killMe();
    modelAdapter = new ModelAdapter(*newModel);
}

types::InternalType* BlockAdapter::getDocContent() const
{
    return doc_content->clone();
}

void BlockAdapter::setDocContent(types::InternalType* v)
{
    doc_content->killMe();
    doc_content = v->clone();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
