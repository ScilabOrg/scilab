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
static const std::wstring orig (L"orig");
static const std::wstring sz (L"sz");
static const std::wstring flip (L"flip");
static const std::wstring theta (L"theta");
static const std::wstring exprs (L"exprs");
static const std::wstring pin (L"pin");
static const std::wstring pout (L"pout");
static const std::wstring pein (L"pein");
static const std::wstring peout (L"peout");
static const std::wstring gr_i (L"gr_i");
static const std::wstring id (L"id");
static const std::wstring in_implicit (L"in_implicit");
static const std::wstring out_implicit (L"out_implicit");
static const std::wstring in_style (L"in_style");
static const std::wstring out_style (L"out_style");
static const std::wstring in_label (L"in_label");
static const std::wstring out_label (L"out_label");
static const std::wstring style (L"style");

static const wchar_t* modelStr = L"model";
static const std::wstring sim (L"sim");
static const std::wstring in (L"in");
static const std::wstring in2 (L"in2");
static const std::wstring intyp (L"intyp");
static const std::wstring out (L"out");
static const std::wstring out2 (L"out2");
static const std::wstring outtyp (L"outtyp");
static const std::wstring evtin (L"evtin");
static const std::wstring evtout (L"evtout");
static const std::wstring state (L"state");
static const std::wstring dstate (L"dstate");
static const std::wstring odstate (L"odstate");
static const std::wstring rpar (L"rpar");
static const std::wstring ipar (L"ipar");
static const std::wstring opar (L"opar");
static const std::wstring blocktype (L"blocktype");
static const std::wstring firing (L"firing");
static const std::wstring dep_ut (L"dep_ut");
static const std::wstring label (L"label");
static const std::wstring nzcross (L"nzcross");
static const std::wstring nmode (L"nmode");
static const std::wstring equations (L"equations");
static const std::wstring uid (L"uid");

struct graphics
{
    static types::InternalType* get(const BlockAdapter& adaptor, const Controller& controller)
    {
        GraphicsAdapter adaptee = GraphicsAdapter(adaptor.getAdaptee());

        types::MList* o = new types::MList();
        types::String* Graphics = new types::String(1, 1);
        Graphics->set(0, graphicsStr);
        o->set(0, Graphics);

        types::InternalType* currentField;
        std::vector<std::wstring> graphicsFields (18);
        graphicsFields[0] = orig;
        graphicsFields[1] = sz;
        graphicsFields[2] = flip;
        graphicsFields[3] = theta;
        graphicsFields[4] = exprs;
        graphicsFields[5] = pin;
        graphicsFields[6] = pout;
        graphicsFields[7] = pein;
        graphicsFields[8] = peout;
        graphicsFields[9] = gr_i;
        graphicsFields[10] = id;
        graphicsFields[11] = in_implicit;
        graphicsFields[12] = out_implicit;
        graphicsFields[13] = in_style;
        graphicsFields[14] = out_style;
        graphicsFields[15] = in_label;
        graphicsFields[16] = out_label;
        graphicsFields[17] = style;

        for (int i = 0; i < 18; ++i)
        {
            //currentField = adaptee.getProperty(graphicsFields[i], controller);
            o->set(i + 1, currentField);
        }

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
            std::vector<std::wstring> graphicsFields (18);
            graphicsFields[0] = orig;
            graphicsFields[1] = sz;
            graphicsFields[2] = flip;
            graphicsFields[3] = theta;
            graphicsFields[4] = exprs;
            graphicsFields[5] = pin;
            graphicsFields[6] = pout;
            graphicsFields[7] = pein;
            graphicsFields[8] = peout;
            graphicsFields[9] = gr_i;
            graphicsFields[10] = id;
            graphicsFields[11] = in_implicit;
            graphicsFields[12] = out_implicit;
            graphicsFields[13] = in_style;
            graphicsFields[14] = out_style;
            graphicsFields[15] = in_label;
            graphicsFields[16] = out_label;
            graphicsFields[17] = style;

            for (int i = 0; i < 18; ++i)
            {
                if ((currentField = current->getField(graphicsFields[i])) == NULL)
                {
                    return false;
                }
                if (!graphics->setProperty(graphicsFields[i], currentField, controller))
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
        GraphicsAdapter adaptee = GraphicsAdapter(adaptor.getAdaptee());

        types::MList* o = new types::MList();
        types::String* Graphics = new types::String(1, 1);
        Graphics->set(0, modelStr);
        o->set(0, Graphics);

        types::InternalType* currentField;
        std::vector<std::wstring> modelFields (23);
        modelFields[0] = sim;
        modelFields[1] = in;
        modelFields[2] = in2;
        modelFields[3] = intyp;
        modelFields[4] = out;
        modelFields[5] = out2;
        modelFields[6] = outtyp;
        modelFields[7] = evtin;
        modelFields[8] = evtout;
        modelFields[9] = state;
        modelFields[10] = dstate;
        modelFields[11] = odstate;
        modelFields[12] = rpar;
        modelFields[13] = ipar;
        modelFields[14] = opar;
        modelFields[15] = blocktype;
        modelFields[16] = firing;
        modelFields[17] = dep_ut;
        modelFields[18] = label;
        modelFields[19] = nzcross;
        modelFields[20] = nmode;
        modelFields[21] = equations;
        modelFields[22] = uid;

        for (int i = 0; i < 23; ++i)
        {
            //currentField = adaptee.getProperty(modelFields[i], controller);
            o->set(i + 1, currentField);
        }

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
            std::vector<std::wstring> modelFields (23);
            modelFields[0] = sim;
            modelFields[1] = in;
            modelFields[2] = in2;
            modelFields[3] = intyp;
            modelFields[4] = out;
            modelFields[5] = out2;
            modelFields[6] = outtyp;
            modelFields[7] = evtin;
            modelFields[8] = evtout;
            modelFields[9] = state;
            modelFields[10] = dstate;
            modelFields[11] = odstate;
            modelFields[12] = rpar;
            modelFields[13] = ipar;
            modelFields[14] = opar;
            modelFields[15] = blocktype;
            modelFields[16] = firing;
            modelFields[17] = dep_ut;
            modelFields[18] = label;
            modelFields[19] = nzcross;
            modelFields[20] = nmode;
            modelFields[21] = equations;
            modelFields[22] = uid;

            for (int i = 0; i < 23; ++i)
            {
                if ((currentElement = current->getField(modelFields[i])) == NULL)
                {
                    return false;
                }
                if (!model->setProperty(modelFields[i], currentElement, controller))
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
