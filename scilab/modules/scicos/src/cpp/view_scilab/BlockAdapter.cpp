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

#include <cwchar>

#include <string>
#include <vector>

#include "internal.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "string.hxx"
#include "types.hxx"

#include "utilities.hxx"
#include "adapters_utilities.hxx"
#include "Controller.hxx"
#include "model/Block.hxx"
#include "BlockAdapter.hxx"
#include "DiagramAdapter.hxx"
#include "GraphicsAdapter.hxx"
#include "LinkAdapter.hxx"
#include "ModelAdapter.hxx"
#include "TextAdapter.hxx"

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
        GraphicsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        types::InternalType* v = localAdaptor.getAsTList(new types::MList(), controller);
        return v;
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        GraphicsAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()));
        return localAdaptor.setAsTList(v, controller);
    }
};

struct model
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        ModelAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()), static_cast<DiagramAdapter*>(adaptor.getDiagram()));
        types::InternalType* mlist = localAdaptor.getAsTList(new types::MList(), controller)->getAs<types::MList>();

        if (localAdaptor.getDiagram() != nullptr)
        {
            // FIXME: the following lines look useless, keep them?
            //const_cast<BlockAdapter&>(adaptor).setFrom(localAdaptor.getDiagram()->getFrom());
            //const_cast<BlockAdapter&>(adaptor).setTo(localAdaptor.getDiagram()->getTo());
            //const_cast<BlockAdapter&>(adaptor).setBlocks(localAdaptor.getDiagram()->getBlocks());
            //const_cast<BlockAdapter&>(adaptor).setContribContent(localAdaptor.getDiagram()->getContribContent());
        }
        return mlist;
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        ModelAdapter localAdaptor(controller, controller.referenceObject(adaptor.getAdaptee()), nullptr);
        if (!localAdaptor.setAsTList(v, controller))
        {
            return false;
        }

        if (localAdaptor.getDiagram() != nullptr)
        {
            adaptor.setFrom(localAdaptor.getDiagram()->getFrom());
            adaptor.setTo(localAdaptor.getDiagram()->getTo());
            adaptor.setBlocks(localAdaptor.getDiagram()->getBlocks());
            adaptor.setContribContent(localAdaptor.getDiagram()->getContribContent());
        }
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
        if (!current->isScalar())
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
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getDocContent();
    }

    static bool set(BlockAdapter& adaptor, types::InternalType* v, Controller& /*controller*/)
    {
        adaptor.setDocContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<BlockAdapter>::props_t property<BlockAdapter>::fields = property<BlockAdapter>::props_t();

BlockAdapter::BlockAdapter(const Controller& c, org_scilab_modules_scicos::model::Block* adaptee) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(c, adaptee),
    doc_content(nullptr),
    from_vec(),
    to_vec(),
    child_blocks(),
    contrib_content(nullptr)
{
    if (property<BlockAdapter>::properties_have_not_been_set())
    {
        property<BlockAdapter>::fields.reserve(4);
        property<BlockAdapter>::add_property(L"graphics", &graphics::get, &graphics::set);
        property<BlockAdapter>::add_property(L"model", &model::get, &model::set);
        property<BlockAdapter>::add_property(L"gui", &gui::get, &gui::set);
        property<BlockAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }

    setDocContent(new types::List());
}

BlockAdapter::BlockAdapter(const BlockAdapter& adapter) :
    BaseAdapter<BlockAdapter, org_scilab_modules_scicos::model::Block>(adapter, false),
    doc_content(nullptr),
    from_vec(adapter.getFrom()),
    to_vec(adapter.getTo()),
    child_blocks(adapter.getBlocks()),
    contrib_content(adapter.getContribContent())
{
    Controller controller;

    if (adapter.getDiagram() != nullptr)
    {
        types::InternalType* model = model::get(adapter, controller);
        model::set(*this, model, controller);
        model->killMe();
    }

    setDocContent(adapter.getDocContent());
}

BlockAdapter::~BlockAdapter()
{
    // CHILDREN will be unreferenced on Controller::deleteObject

    for (BlockAdapter * child : child_blocks)
    {
        child->DecreaseRef();
        child->killMe();
    }

    doc_content->DecreaseRef();
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

types::InternalType* BlockAdapter::getDocContent() const
{
    return doc_content;
}

void BlockAdapter::setDocContent(types::InternalType* v)
{
    types::InternalType* temp = doc_content;

    v->IncreaseRef();
    doc_content = v;

    if (temp != nullptr)
    {
        temp->DecreaseRef();
        temp->killMe();
    }
}

types::UserType* BlockAdapter::getDiagram() const
{
    Controller controller;
    std::vector<ScicosID> children;
    controller.getObjectProperty(getAdaptee()->id(), BLOCK, CHILDREN, children);
    if (!children.empty())
    {
        ScicosID newDiag = controller.createObject(DIAGRAM);
        DiagramAdapter* diagram = new DiagramAdapter(controller, static_cast<org_scilab_modules_scicos::model::Diagram*>(controller.getObject(newDiag)));
        controller.setObjectProperty(newDiag, DIAGRAM, CHILDREN, children);
        for (const ScicosID id : children)
        {
            org_scilab_modules_scicos::model::BaseObject* o = controller.getObject(id);
            controller.setObjectProperty(o->id(), o->kind(), PARENT_DIAGRAM, newDiag);
            controller.referenceObject(o->id());
        }
        diagram->setFrom(from_vec);
        diagram->setTo(to_vec);
        diagram->setBlocks(child_blocks);
        diagram->setContribContent(contrib_content);

        std::vector<std::string> context;
        controller.getObjectProperty(getAdaptee()->id(), BLOCK, DIAGRAM_CONTEXT, context);
        controller.setObjectProperty(newDiag, DIAGRAM, DIAGRAM_CONTEXT, context);
        return diagram;
    }
    else
    {
        return nullptr;
    }
}

std::vector<link_t> BlockAdapter::getFrom() const
{
    return from_vec;
}

void BlockAdapter::setFrom(const std::vector<link_t>& from)
{
    from_vec = from;
}

std::vector<link_t> BlockAdapter::getTo() const
{
    return to_vec;
}

void BlockAdapter::setTo(const std::vector<link_t>& to)
{
    to_vec = to;
}

std::vector<BlockAdapter*> BlockAdapter::getBlocks() const
{
    return child_blocks;
}

void BlockAdapter::setBlocks(const std::vector<BlockAdapter*>& blocks)
{
    std::vector<BlockAdapter*> temp = child_blocks;

    child_blocks = blocks;
    for (BlockAdapter * child : child_blocks)
    {
        child->IncreaseRef();
    }
    for (BlockAdapter * child : temp)
    {
        child->DecreaseRef();
        child->killMe();
    }
}

types::InternalType* BlockAdapter::getContribContent() const
{
    return contrib_content;
}

void BlockAdapter::setContribContent(types::InternalType* contrib)
{
    contrib_content = contrib;
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
