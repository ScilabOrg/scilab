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

#include "double.hxx"
#include "string.hxx"
#include "mlist.hxx"
#include "internal.hxx"

#include "Controller.hxx"
#include "TextAdapter.hxx"
#include "GraphicsAdapter.hxx"

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

const std::wstring Graphics(L"graphics");
const std::wstring orig(L"orig");
const std::wstring sz(L"sz");
const std::wstring exprs(L"exprs");

struct graphics
{
    static types::InternalType* get(const TextAdapter& adaptor, const Controller& /*controller*/)
    {
        return adaptor.getGraphics();
    }

    static bool set(TextAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabMList)
        {
            return false;
        }

        types::MList* current = v->getAs<types::MList>();
        if (current->getSize() < 4)
        {
            return false;
        }
        types::MList* toAdapter = new types::MList(); // At the same time that we extract 'graphics', copy it to this Adapter
        types::String* header = new types::String(1, 4);
        header->set(0, Graphics.c_str());
        header->set(1, orig.c_str());
        header->set(2, sz.c_str());
        header->set(3, exprs.c_str());
        toAdapter->set(0, header);

        types::InternalType* currentField;
        types::Double* currentFieldDouble;
        types::String* currentFieldString;

        model::Annotation* adaptee = adaptor.getAdaptee();

        // orig
        if ((currentField = current->getField(orig.c_str())) == NULL)
        {
            return false;
        }
        if (currentField->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }
        currentFieldDouble = currentField->getAs<types::Double>();
        if (currentFieldDouble->getRows() != 1 || currentFieldDouble->getCols() != 2)
        {
            return false;
        }
        std::vector<double> origField;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, origField);
        origField[0] = currentFieldDouble->get(0);
        origField[1] = currentFieldDouble->get(1);
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, origField);

        toAdapter->set(1, currentFieldDouble);

        // sz
        if ((currentField = current->getField(sz.c_str())) == NULL)
        {
            return false;
        }
        if (currentField->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }
        currentFieldDouble = currentField->getAs<types::Double>();
        if (currentFieldDouble->getRows() != 1 || currentFieldDouble->getCols() != 2)
        {
            return false;
        }
        std::vector<double> szField;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, szField);
        szField[2] = currentFieldDouble->get(0);
        szField[3] = currentFieldDouble->get(1);
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, szField);

        toAdapter->set(2, currentFieldDouble);

        // exprs
        if ((currentField = current->getField(exprs.c_str())) == NULL)
        {
            return false;
        }
        if (currentField->getType() == types::InternalType::ScilabString)
        {
            currentFieldString = currentField->getAs<types::String>();
            if (currentFieldString->getCols() != 1 || currentFieldString->getSize() != 3)
            {
                return false;
            }

            std::vector<std::string> exprsField (3);
            for (int i = 0; i < (int) exprsField.size(); ++i)
            {
                char* c_str = wide_string_to_UTF8(currentFieldString->get(i));
                exprsField[i] = std::string(c_str);
                FREE(c_str);
            }
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), DESCRIPTION, exprsField[0]);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), FONT, exprsField[1]);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), FONT_SIZE, exprsField[2]);

            toAdapter->set(3, currentFieldString);
            adaptor.setGraphics(toAdapter);
            return true;
        }
        else if (currentField->getType() == types::InternalType::ScilabDouble)
        {
            currentFieldDouble = currentField->getAs<types::Double>();
            if (currentFieldDouble->getSize() != 0)
            {
                return false;
            }

            std::vector<std::string> exprsField (3);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), DESCRIPTION, exprsField[0]);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), FONT, exprsField[1]);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), FONT_SIZE, exprsField[2]);

            toAdapter->set(3, currentFieldDouble);
            adaptor.setGraphics(toAdapter);
            return true;
        }

        return false;
    }
};

struct dummy_property
{

    static types::InternalType* get(const TextAdapter& /*adaptor*/, const Controller& /*controller*/)
    {
        // Return an empty matrix because this field isn't used.
        return types::Double::Empty();
    }

    static bool set(TextAdapter& /*adaptor*/, types::InternalType* /*v*/, Controller& /*controller*/)
    {
        // everything should be right as the properties mapped using this adapter do not perform anything
        return true;
    }
};

} /* namespace */

template<> property<TextAdapter>::props_t property<TextAdapter>::fields = property<TextAdapter>::props_t();

TextAdapter::TextAdapter(bool ownAdaptee, org_scilab_modules_scicos::model::Annotation* adaptee) :
    BaseAdapter<TextAdapter, org_scilab_modules_scicos::model::Annotation>(ownAdaptee, adaptee)
{
    if (property<TextAdapter>::properties_have_not_been_set())
    {
        property<TextAdapter>::fields.reserve(4);
        property<TextAdapter>::add_property(Graphics, &graphics::get, &graphics::set);
        property<TextAdapter>::add_property(L"model", &dummy_property::get, &dummy_property::set);
        property<TextAdapter>::add_property(L"void", &dummy_property::get, &dummy_property::set);
        property<TextAdapter>::add_property(L"gui", &dummy_property::get, &dummy_property::set);
    }

    graphicsAdapter = new types::MList();
}

TextAdapter::~TextAdapter()
{
    delete graphicsAdapter;
}

std::wstring TextAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring TextAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* TextAdapter::getGraphics() const
{
    return graphicsAdapter->clone();
}

void TextAdapter::setGraphics(types::InternalType* newGraphics)
{
    delete graphicsAdapter;
    graphicsAdapter = newGraphics->clone();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
