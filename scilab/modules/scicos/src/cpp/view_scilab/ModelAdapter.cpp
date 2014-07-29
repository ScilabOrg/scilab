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

#include "list.hxx"
#include "double.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "ModelAdapter.hxx"
#include "ports_management.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

struct sim
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        // First, extact the function Name
        std::string name;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_NAME, name);
        types::String* Name = new types::String(1, 1);
        Name->set(0, name.data());

        // Then the Api. If it is zero, then just return the Name. Otherwise, return a list containing both.
        int api;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_API, api);

        if (api == 0)
        {
            return Name;
        }
        else
        {
            types::Double* Api = new types::Double(1, 1, (double) api);
            types::List* o = new types::List();
            o->set(0, Name);
            o->set(1, Api);
            return o;
        }
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        if (v->getType() == types::InternalType::ScilabString)
        {
            types::String* current = v->getAs<types::String>();
            if (current->getSize() != 1)
            {
                return false;
            }

            char* c_str = wide_string_to_UTF8(current->get(0));
            std::string name = std::string(c_str);
            FREE(c_str);

            // If the input is a scalar string, then the functionApi is 0.
            int api = 0;

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_NAME, name);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_API, api);
        }
        else if (v->getType() == types::InternalType::ScilabList)
        {
            // If the input is a 2-sized list, then it must be string and positive integer. Extract and set them.
            types::List* current = v->getAs<types::List>();
            if (current->getSize() != 2)
            {
                return false;
            }
            if (current->get(0)->getType() != types::InternalType::ScilabString || current->get(1)->getType() != types::InternalType::ScilabDouble)
            {
                return false;
            }

            types::String* Name = current->get(0)->getAs<types::String>();
            if (Name->getSize() != 1)
            {
                return false;
            }
            char* c_str = wide_string_to_UTF8(Name->get(0));
            std::string name = std::string(c_str);
            FREE(c_str);

            types::Double* Api = current->get(1)->getAs<types::Double>();
            if (Api->getSize() != 1)
            {
                return false;
            }
            int api = Api->get(0);
            if (floor(api) != api)
            {
                return false;
            }
            if (api < 0 || api > 5)
            {
                return false;
            }

            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_NAME, name);
            controller.setObjectProperty(adaptee->id(), adaptee->kind(), SIM_FUNCTION_API, api);
        }
        else
        {
            return false;
        }
        return true;
    }
};

static bool set_port(const ModelAdapter& adaptor, types::InternalType* v, object_properties_t p, Controller& controller, object_properties_t p2)
{
    if (v->getType() != types::InternalType::ScilabDouble)
    {
        return false;
    }

    model::Block* adaptee = adaptor.getAdaptee();

    types::Double* current = v->getAs<types::Double>();
    if (current->getCols() != 0 && current->getCols() != 1)
    {
        return false;
    }

    // Retrieve the ports i dentifiers
    std::vector<ScicosID> ids;
    size_t rows = current->getRows();
    controller.getObjectProperty(adaptee->id(), adaptee->kind(), p, ids);
    if (rows != ids.size())
    {
        return false;
    }

    int i = 0;
    std::vector<double> data = std::vector<double>(current->getSize());
    for (std::vector<ScicosID>::iterator it = ids.begin(); it != ids.end(); ++it, ++i)
    {
        data[i] = current->get(i);
        controller.setObjectProperty(*it, PORT, p2, data[i]);
    }

    return true;
}

struct inSize1
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter>(adaptor, INPUTS, controller, INPUTS_SIZE1);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_with_property<ModelAdapter>(adaptor, INPUTS, controller, INPUTS_SIZE1, v);
    }
};

struct inSize2
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter>(adaptor, INPUTS, controller, INPUTS_SIZE2);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_with_property<ModelAdapter>(adaptor, INPUTS, controller, INPUTS_SIZE2, v);
    }
};

struct inType
{

    static types::InternalType* get(const ModelAdapter& adaptor, const Controller& controller)
    {
        return get_ports_property<ModelAdapter>(adaptor, INPUTS, controller, INPUTS_TYPE);
    }

    static bool set(ModelAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        return update_ports_with_property<ModelAdapter>(adaptor, INPUTS, controller, INPUTS_TYPE, v);
    }
};

template<> property<ModelAdapter>::props_t property<ModelAdapter>::fields = property<ModelAdapter>::props_t();

ModelAdapter::ModelAdapter(const ModelAdapter& o) :
    BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>(o) { }

ModelAdapter::ModelAdapter(org_scilab_modules_scicos::model::Block* o) :
    BaseAdapter<ModelAdapter, org_scilab_modules_scicos::model::Block>(o)
{
    if (property<ModelAdapter>::properties_has_not_been_set())
    {
        property<ModelAdapter>::fields.reserve(4);
        property<ModelAdapter>::add_property(L"sim", &sim::get, &sim::set);
        property<ModelAdapter>::add_property(L"inSize1", &inSize1::get, &inSize1::set);
        property<ModelAdapter>::add_property(L"inSize2", &inSize2::get, &inSize2::set);
        property<ModelAdapter>::add_property(L"inType", &inType::get, &inType::set);
    }
}

ModelAdapter::~ModelAdapter()
{
}

bool ModelAdapter::toString(std::wostringstream& ostr)
{
    ostr << L"ModelAdapter.hxx: Dunno what to display there" << std::endl;
    return true;
}

std::wstring ModelAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring ModelAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
