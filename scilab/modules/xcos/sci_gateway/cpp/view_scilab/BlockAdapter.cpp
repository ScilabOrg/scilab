/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string>

#include "internal.hxx"
#include "list.hxx"
#include "string.hxx"
#include "types.hxx"
#include "user.hxx"

#include "BlockAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "ModelAdapter.hxx"

namespace org_scilab_modules_xcos
{
namespace view_scilab
{

struct graphics
{

    static types::InternalType* get(const BlockAdapter& adaptor)
    {
        return new GraphicsAdapter(adaptor.get_adaptee());
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() == types::InternalType::ScilabUserType
                && v->getShortTypeStr() == GraphicsAdapter::getSharedTypeStr())
        {
            GraphicsAdapter* graphics = v->getAs<GraphicsAdapter>();
            adaptor.set_adaptee(graphics->get_adaptee());
            return true;
        }
        return false;
    }
};

struct model
{

    static types::InternalType* get(const BlockAdapter& adaptor)
    {
        return new ModelAdapter(adaptor.get_adaptee());
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() == types::InternalType::ScilabUserType
                && v->getShortTypeStr() == ModelAdapter::getSharedTypeStr())
        {
            ModelAdapter* model = v->getAs<ModelAdapter>();
            adaptor.set_adaptee(model->get_adaptee());
            return true;
        }
        return false;
    }
};

struct gui
{

    static types::InternalType* get(const BlockAdapter& adaptor)
    {
        return new types::String(adaptor.get_adaptee()->get_typename().data());
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v)
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

        adaptor.get_adaptee()->set_typename(current->get(0));
        return true;
    }
};

struct doc
{

    static types::InternalType* get(const BlockAdapter& adaptor)
    {
        return adaptor.get_doc_content();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v)
    {
        adaptor.set_doc_content(v->clone());
        return true;
    }
};

BlockAdapter::BlockAdapter(const BlockAdapter& o) :
    BaseAdapter<BlockAdapter, org_scilab_modules_xcos::model::Block>(o), doc_content(o.doc_content->clone()) { }

BlockAdapter::BlockAdapter(org_scilab_modules_xcos::model::Block* o) :
    BaseAdapter<BlockAdapter, org_scilab_modules_xcos::model::Block>(o)
{
    if (properties_has_not_been_set())
    {
        add_property(L"graphics", &graphics::get, &graphics::set);
        add_property(L"model", &model::get, &model::set);
        add_property(L"gui", &gui::get, &gui::set);
        add_property(L"doc", &doc::get, &doc::set);
    }

    doc_content = new types::List();
}

BlockAdapter::~BlockAdapter()
{
    delete doc_content;
}

} /* view_scilab */
} /* namespace org_scilab_modules_xcos */
