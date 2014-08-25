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

static const wchar_t* graphicsStr = L"graphics";
static const wchar_t* orig = L"orig";
static const wchar_t* sz = L"sz";
static const wchar_t* flip = L"flip";
static const wchar_t* theta = L"theta";
static const wchar_t* exprs = L"exprs";
static const wchar_t* pin = L"pin";
static const wchar_t* pout = L"pout";
static const wchar_t* pein = L"pein";
static const wchar_t* peout = L"peout";
static const wchar_t* gr_i = L"gr_i";
static const wchar_t* id = L"id";
static const wchar_t* in_implicit = L"in_implicit";
static const wchar_t* out_implicit = L"out_implicit";
static const wchar_t* in_style = L"in_style";
static const wchar_t* out_style = L"out_style";
static const wchar_t* in_label = L"in_label";
static const wchar_t* out_label = L"out_label";
static const wchar_t* style = L"style";

static const wchar_t* modelStr = L"model";
static const wchar_t* sim = L"sim";
static const wchar_t* in = L"in";
static const wchar_t* in2 = L"in2";
static const wchar_t* intyp = L"intyp";
static const wchar_t* out = L"out";
static const wchar_t* out2 = L"out2";
static const wchar_t* outtyp = L"outtyp";
static const wchar_t* evtin = L"evtin";
static const wchar_t* evtout = L"evtout";
static const wchar_t* state = L"state";
static const wchar_t* dstate = L"dstate";
static const wchar_t* odstate = L"odstate";
static const wchar_t* rpar = L"rpar";
static const wchar_t* ipar = L"ipar";
static const wchar_t* opar = L"opar";
static const wchar_t* blocktype = L"blocktype";
static const wchar_t* firing = L"firing";
static const wchar_t* dep_ut = L"dep_ut";
static const wchar_t* label = L"label";
static const wchar_t* nzcross = L"nzcross";
static const wchar_t* nmode = L"nmode";
static const wchar_t* equations = L"equations";
static const wchar_t* uid = L"uid";

struct graphics
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        GraphicsAdapter adaptee = GraphicsAdapter(adaptor.getAdaptee());

        types::MList* o = new types::MList();
        types::String* MListFields = new types::String(1, 19);
        MListFields->set(0, graphicsStr);
        MListFields->set(1, orig);
        MListFields->set(2, sz);
        MListFields->set(3, flip);
        MListFields->set(4, theta);
        MListFields->set(5, exprs);
        MListFields->set(6, pin);
        MListFields->set(7, pout);
        MListFields->set(8, pein);
        MListFields->set(9, peout);
        MListFields->set(10, gr_i);
        MListFields->set(11, id);
        MListFields->set(12, in_implicit);
        MListFields->set(13, out_implicit);
        MListFields->set(14, in_style);
        MListFields->set(15, out_style);
        MListFields->set(16, in_label);
        MListFields->set(17, out_label);
        MListFields->set(18, style);
        o->set(0, MListFields);

        types::InternalType* currentField;

        //currentField = adaptee.getProperty(orig, controller);
        o->set(orig, currentField);
        //currentField = adaptee.getProperty(sz, controller);
        o->set(sz, currentField);
        //currentField = adaptee.getProperty(flip, controller);
        o->set(flip, currentField);
        //currentField = adaptee.getProperty(theta, controller);
        o->set(theta, currentField);
        //currentField = adaptee.getProperty(exprs, controller);
        o->set(exprs, currentField);
        //currentField = adaptee.getProperty(pin, controller);
        o->set(pin, currentField);
        //currentField = adaptee.getProperty(pout, controller);
        o->set(pout, currentField);
        //currentField = adaptee.getProperty(pein, controller);
        o->set(pein, currentField);
        //currentField = adaptee.getProperty(peout, controller);
        o->set(peout, currentField);
        //currentField = adaptee.getProperty(gr_i, controller);
        o->set(gr_i, currentField);
        //currentField = adaptee.getProperty(id, controller);
        o->set(id, currentField);
        //currentField = adaptee.getProperty(in_implicit, controller);
        o->set(in_implicit, currentField);
        //currentField = adaptee.getProperty(out_implicit, controller);
        o->set(out_implicit, currentField);
        //currentField = adaptee.getProperty(in_style, controller);
        o->set(in_style, currentField);
        //currentField = adaptee.getProperty(out_style, controller);
        o->set(out_style, currentField);
        //currentField = adaptee.getProperty(in_label, controller);
        o->set(in_label, currentField);
        //currentField = adaptee.getProperty(out_label, controller);
        o->set(out_label, currentField);
        //currentField = adaptee.getProperty(style, controller);
        o->set(style, currentField);

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

            types::InternalType* currentField;

            if ((currentField = current->getField(orig)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(orig, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(sz)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(sz, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(flip)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(flip, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(theta)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(theta, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(exprs)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(exprs, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(pin)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(pin, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(pout)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(pout, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(pein)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(pein, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(peout)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(peout, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(in_implicit)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(in_implicit, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(out_implicit)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(out_implicit, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(in_style)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(in_style, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(out_style)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(out_style, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(in_label)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(in_label, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(out_label)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(out_label, currentField, controller))
            {
                return false;
            }
            if ((currentField = current->getField(style)) == NULL)
            {
                return false;
            }
            if (!graphics->setProperty(style, currentField, controller))
            {
                return false;
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
        ModelAdapter adaptee = ModelAdapter(adaptor.getAdaptee());

        types::MList* o = new types::MList();
        types::String* ModelFields = new types::String(1, 24);
        ModelFields->set(0, modelStr);
        ModelFields->set(1, sim);
        ModelFields->set(2, in);
        ModelFields->set(3, in2);
        ModelFields->set(4, intyp);
        ModelFields->set(5, out);
        ModelFields->set(6, out2);
        ModelFields->set(7, outtyp);
        ModelFields->set(8, evtin);
        ModelFields->set(9, evtout);
        ModelFields->set(10, state);
        ModelFields->set(11, dstate);
        ModelFields->set(12, odstate);
        ModelFields->set(13, rpar);
        ModelFields->set(14, ipar);
        ModelFields->set(15, opar);
        ModelFields->set(16, blocktype);
        ModelFields->set(17, firing);
        ModelFields->set(18, dep_ut);
        ModelFields->set(19, label);
        ModelFields->set(20, nzcross);
        ModelFields->set(21, nmode);
        ModelFields->set(22, equations);
        ModelFields->set(23, uid);
        o->set(0, ModelFields);

        types::InternalType* currentField;

        //currentField = adaptee.getProperty(sim, controller);
        o->set(sim, currentField);
        //currentField = adaptee.getProperty(in, controller);
        o->set(in, currentField);
        //currentField = adaptee.getProperty(in2, controller);
        o->set(in2, currentField);
        //currentField = adaptee.getProperty(intyp, controller);
        o->set(intyp, currentField);
        //currentField = adaptee.getProperty(out, controller);
        o->set(out, currentField);
        //currentField = adaptee.getProperty(out2, controller);
        o->set(out2, currentField);
        //currentField = adaptee.getProperty(outtyp, controller);
        o->set(outtyp, currentField);
        //currentField = adaptee.getProperty(evtin, controller);
        o->set(evtin, currentField);
        //currentField = adaptee.getProperty(evtout, controller);
        o->set(evtout, currentField);
        //currentField = adaptee.getProperty(state, controller);
        o->set(state, currentField);
        //currentField = adaptee.getProperty(dstate, controller);
        o->set(dstate, currentField);
        //currentField = adaptee.getProperty(odstate, controller);
        o->set(odstate, currentField);
        //currentField = adaptee.getProperty(rpar, controller);
        o->set(rpar, currentField);
        //currentField = adaptee.getProperty(ipar, controller);
        o->set(ipar, currentField);
        //currentField = adaptee.getProperty(opar, controller);
        o->set(opar, currentField);
        //currentField = adaptee.getProperty(blocktype, controller);
        o->set(blocktype, currentField);
        //currentField = adaptee.getProperty(firing, controller);
        o->set(firing, currentField);
        //currentField = adaptee.getProperty(dep_ut, controller);
        o->set(dep_ut, currentField);
        //currentField = adaptee.getProperty(label, controller);
        o->set(label, currentField);
        //currentField = adaptee.getProperty(nzcross, controller);
        o->set(nzcross, currentField);
        //currentField = adaptee.getProperty(nmode, controller);
        o->set(nmode, currentField);
        //currentField = adaptee.getProperty(equations, controller);
        o->set(equations, currentField);
        //currentField = adaptee.getProperty(uid, controller);
        o->set(uid, currentField);

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

            types::InternalType* currentElement;

            if ((currentElement = current->getField(sim)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(sim, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(in)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(in, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(in2)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(in2, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(intyp)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(intyp, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(out)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(out, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(out2)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(out2, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(outtyp)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(outtyp, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(evtin)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(evtin, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(evtout)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(evtout, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(state)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(state, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(dstate)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(dstate, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(odstate)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(odstate, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(rpar)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(rpar, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(ipar)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(ipar, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(opar)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(opar, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(blocktype)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(blocktype, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(firing)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(firing, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(dep_ut)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(dep_ut, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(label)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(label, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(nzcross)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(nzcross, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(nmode)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(nmode, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(equations)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(equations, currentElement, controller))
            {
                return false;
            }
            if ((currentElement = current->getField(uid)) == NULL)
            {
                return false;
            }
            if (!model->setProperty(uid, currentElement, controller))
            {
                return false;
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
    if (property<BlockAdapter>::properties_have_not_been_set())
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
