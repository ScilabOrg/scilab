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
#include <algorithm>

#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "tlist.hxx"

#include "Controller.hxx"
#include "ParamsAdapter.hxx"

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

const std::wstring scsopt(L"scsopt");

struct dummy_property
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) adaptor;
        (void) controller;

        // Return a default empty matrix.
        return types::Double::Empty();
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // silent unused parameter warnings
        (void) adaptor;
        (void) v;
        (void) controller;

        // everything should be right as the properties mapped using this adapter do not perform anything
        return true;
    }
};

struct title
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::string title;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), TITLE, title);
        std::string path;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PATH, path);

        types::String* o = new types::String(2, 1);
        o->set(0, title.data());
        o->set(1, path.data());

        return o;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        std::string path;
        std::string title;
        types::String* current = v->getAs<types::String>();
        if (current->getSize() == 1)
        {
            // Pass an empty path
        }
        else if (current->getSize() == 2)
        {
            char* Path = wide_string_to_UTF8(current->get(1));
            title = std::string(Path);
            FREE(Path);
        }
        else
        {
            return false;
        }

        char* Title = wide_string_to_UTF8(current->get(0));
        title = std::string(Title);
        FREE(Title);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), TITLE, title);
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), PATH, path);
        return true;
    }
};

struct tol
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        double* data;
        types::Double* o = new types::Double(1, 7, &data);

        std::vector<double> tol;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);
#ifdef _MSC_VER
        std::copy(tol.begin() + 1, tol.end(), stdext::checked_array_iterator<double*>( data, 7 ));
#else
        std::copy(tol.begin() + 1, tol.end(), data);
#endif

        return o;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 7)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<double> tol;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);

        std::copy(current->getReal(), current->getReal() + current->getSize(), tol.begin() + 1);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);
        return true;
    }
};

struct tf
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<double> tf;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tf);

        return new types::Double(tf[0]);
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getSize() != 1)
        {
            return false;
        }

        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<double> tol;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);

        tol[0] = current->get(0);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), PROPERTIES, tol);
        return true;
    }
};

struct context
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        model::Diagram* adaptee = adaptor.getAdaptee();

        std::vector<std::string> context;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), DIAGRAM_CONTEXT, context);

        if (context.size() == 0)
        {
            // An empty context returns an empty matrix
            return types::Double::Empty();
        }

        types::String* o = new types::String((int)context.size(), 1);
        for (int i = 0; i < (int)context.size(); ++i)
        {
            o->set(i, context[i].data());
        }

        return o;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getCols() != 0 && current->getCols() != 1)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee();

            std::vector<std::string> context (current->getSize());
            for (int i = 0; i < (int)context.size(); ++i)
            {
                char* c_str = wide_string_to_UTF8(current->get(i));
                context[i] = std::string(c_str);
                FREE(c_str);
            }

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), DIAGRAM_CONTEXT, context);
            return true;
        }
        else if (v->getType() == types::InternalType::ScilabDouble)
        {
            types::Double* current = v->getAs<types::Double>();
            if (current->getRows() != 0 || current->getCols() != 0)
            {
                return false;
            }

            model::Diagram* adaptee = adaptor.getAdaptee();

            std::vector<std::string> context;
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), DIAGRAM_CONTEXT, context);
            return true;
        }
        return false;
    }
};

struct options
{

    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) adaptor;
        (void) controller;

        // Return a dummy 'scsopt'-typed tlist.
        types::String* header = new types::String(scsopt.c_str());

        types::TList* Scsopt = new types::TList();
        Scsopt->set(0, header);
        return Scsopt;
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // The model does not store 'options'.
        return dummy_property::set(adaptor, v, controller);
    }
};

struct doc
{
    static types::InternalType* get(const ParamsAdapter& adaptor, const Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        return adaptor.getDocContent();
    }

    static bool set(ParamsAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        adaptor.setDocContent(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<ParamsAdapter>::props_t property<ParamsAdapter>::fields = property<ParamsAdapter>::props_t();

ParamsAdapter::ParamsAdapter(bool ownAdaptee, org_scilab_modules_scicos::model::Diagram* adaptee) :
    BaseAdapter<ParamsAdapter, org_scilab_modules_scicos::model::Diagram>(ownAdaptee, adaptee)
{
    if (property<ParamsAdapter>::properties_have_not_been_set())
    {
        property<ParamsAdapter>::fields.reserve(10);
        property<ParamsAdapter>::add_property(L"wpar", &dummy_property::get, &dummy_property::set);
        property<ParamsAdapter>::add_property(L"title", &title::get, &title::set);
        property<ParamsAdapter>::add_property(L"tol", &tol::get, &tol::set);
        property<ParamsAdapter>::add_property(L"tf", &tf::get, &tf::set);
        property<ParamsAdapter>::add_property(L"context", &context::get, &context::set);
        property<ParamsAdapter>::add_property(L"void1", &dummy_property::get, &dummy_property::set);
        property<ParamsAdapter>::add_property(L"options", &options::get, &options::set);
        property<ParamsAdapter>::add_property(L"void2", &dummy_property::get, &dummy_property::set);
        property<ParamsAdapter>::add_property(L"void3", &dummy_property::get, &dummy_property::set);
        property<ParamsAdapter>::add_property(L"doc", &doc::get, &doc::set);
    }

    doc_content = new types::List();
}

ParamsAdapter::~ParamsAdapter()
{
    delete doc_content;
}

std::wstring ParamsAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring ParamsAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

void ParamsAdapter::setAdapterContent(ParamsAdapter* oldAdaptor)
{
    setDocContent(oldAdaptor->getDocContent());
}

types::InternalType* ParamsAdapter::getDocContent() const
{
    return doc_content->clone();
}

void ParamsAdapter::setDocContent(types::InternalType* v)
{
    delete doc_content;
    doc_content = v->clone();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
