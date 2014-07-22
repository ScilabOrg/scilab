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
#include <vector>
#include <cassert>

#include "int.hxx"
#include "double.hxx"
#include "string.hxx"

#include "Controller.hxx"
#include "GraphicsAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

struct orig
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> geom;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);

        data[0] = geom[0];
        data[1] = geom[1];

        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> geom;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);

        geom[0] = current->get(0);
        geom[1] = current->get(1);

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);
        return true;
    }
};

struct sz
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> geom;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);

        data[0] = geom[2];
        data[1] = geom[3];
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> geom;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);

        geom[2] = current->get(0);
        geom[3] = current->get(1);

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, geom);
        return true;
    }
};

struct flip
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        int* data;
        types::Bool* o = new types::Bool(1, 1, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> angle;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);

        data[0] = angle[0];
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() != types::InternalType::ScilabBool)
        {
            return false;
        }

        types::Bool* current = v->getAs<types::Bool>();
        if (current->isScalar() != true)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> angle;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);

        angle[0] = (current->get(0) == false) ? 0 : 1;

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);
        return true;
    }
};

struct theta
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        double* data;
        types::Double* o = new types::Double(1, 1, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<double> angle;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);

        data[0] = angle[1];
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->isScalar() != true)
        {
            return false;
        }

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<double> angle;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);

        angle[1] = current->get(0);

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, angle);
        return true;
    }
};

struct exprs
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<std::string> exprs;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, exprs);

        types::String* o = new types::String(exprs.size(), 1);

        for (size_t i = 0; i < exprs.size(); ++i)
        {
            o->set(i, exprs[i].data());
        }
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() == types::InternalType::ScilabString)
        {

            types::String* current = v->getAs<types::String>();
            if (current->getCols() != 0 && current->getCols() != 1)
            {
                return false;
            }

            model::Block* adaptee = adaptor.getAdaptee();
            std::vector<std::string> exprs;
            Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, exprs);

            exprs.resize(current->getRows());
            for (int i = 0; i < exprs.size(); ++i)
            {
                exprs[i] = (char*) current->get(i);
            }

            Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, exprs);
            return true;
        }

        if (v->getType() == types::InternalType::ScilabDouble)
        {

            types::Double* current = v->getAs<types::Double>();
            if (current->getRows() != 0 || current->getCols() != 0)
            {
                return false;
            }

            model::Block* adaptee = adaptor.getAdaptee();
            std::vector<std::string> exprs;
            Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, exprs);

            exprs.clear();

            Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, exprs);
            return true;
        }
    }
};

struct pin
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> ids;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), INPUTS, ids);

        types::Double* o = new types::Double(ids.size(), 1);

        for (size_t i = 0; i < ids.size(); ++i)
        {
            o->set(i, ids[i]);
        }
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<ScicosID> ids;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), INPUTS, ids);

        ids.resize(current->getSize());
        for (int i = 0; i < ids.size(); ++i)
        {
            ids[i] = current->get(i);
        }

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), INPUTS, ids);
        return true;
    }
};

struct pout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::vector<ScicosID> ids;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), OUTPUTS, ids);

        types::Double* o = new types::Double(ids.size(), 1);

        for (size_t i = 0; i < ids.size(); ++i)
        {
            o->set(i, ids[i]);
        }
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        model::Block* adaptee = adaptor.getAdaptee();
        std::vector<ScicosID> ids;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), OUTPUTS, ids);

        ids.resize(current->getSize());
        for (int i = 0; i < ids.size(); ++i)
        {
            ids[i] = current->get(i);
        }

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), OUTPUTS, ids);
        return true;
    }
};

struct pein
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        ScicosID* eids;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), EVENT_INPUTS, &len, &eids);

        types::UInt64* o = new types::UInt64(len, 1);

        for (size_t i = 0; i < len; ++i)
        {
            o->set(i, eids[i]);
        }
        delete[] eids;
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() != types::InternalType::ScilabUInt64)
        {
            return false;
        }

        types::UInt64* current = v->getAs<types::UInt64>();

        model::Block* adaptee = adaptor.getAdaptee();
        ScicosID* eids;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), EVENT_INPUTS, &len, &eids);

        for (size_t i = 0; i < len; ++i)
        {
            eids[i] = current->get(i);
        }

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), EVENT_INPUTS, len, eids);
        delete[] eids;
        return true;
    }
};

struct peout
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        ScicosID* eids;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), EVENT_OUTPUTS, &len, &eids);

        types::UInt64* o = new types::UInt64(len, 1);

        for (size_t i = 0; i < len; ++i)
        {
            o->set(i, eids[i]);
        }
        delete[] eids;
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() != types::InternalType::ScilabUInt64)
        {
            return false;
        }

        types::UInt64* current = v->getAs<types::UInt64>();

        model::Block* adaptee = adaptor.getAdaptee();
        ScicosID* eids;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), EVENT_OUTPUTS, &len, &eids);

        for (size_t i = 0; i < len; ++i)
        {
            eids[i] = current->get(i);
        }

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), EVENT_OUTPUTS, len, eids);
        delete[] eids;
        return true;
    }
};

struct in_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        //FIXME: implement
        return false;
    }
};

struct out_implicit
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        //FIXME: implement
        return false;
    }
};

struct in_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        //FIXME: implement
        return false;
    }
};

struct out_style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        //FIXME: implement
        return false;
    }
};

struct in_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        //FIXME: implement
        return false;
    }
};

struct out_label
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        //FIXME: implement
        return false;
    }
};

struct style
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        //FIXME: implement
        return 0;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        //FIXME: implement
        return false;
    }
};

template<> property<GraphicsAdapter>::props_t property<GraphicsAdapter>::fields = property<GraphicsAdapter>::props_t();

GraphicsAdapter::GraphicsAdapter(const GraphicsAdapter& o) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(o) { }

GraphicsAdapter::GraphicsAdapter(org_scilab_modules_scicos::model::Block* o) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_scicos::model::Block>(o)
{
    if (property<GraphicsAdapter>::properties_has_not_been_set())
    {
        property<GraphicsAdapter>::fields.reserve(15);
        property<GraphicsAdapter>::add_property(L"orig", &orig::get, &orig::set);
        property<GraphicsAdapter>::add_property(L"sz", &sz::get, &sz::set);
        property<GraphicsAdapter>::add_property(L"flip", &flip::get, &flip::set);
        property<GraphicsAdapter>::add_property(L"theta", &theta::get, &theta::set);
        property<GraphicsAdapter>::add_property(L"exprs", &exprs::get, &exprs::set);
        property<GraphicsAdapter>::add_property(L"pin", &pin::get, &pin::set);
        property<GraphicsAdapter>::add_property(L"pout", &pout::get, &pout::set);
        property<GraphicsAdapter>::add_property(L"pein", &pein::get, &pein::set);
        property<GraphicsAdapter>::add_property(L"peout", &peout::get, &peout::set);
        property<GraphicsAdapter>::add_property(L"in_implicit", &in_implicit::get, &in_implicit::set);
        property<GraphicsAdapter>::add_property(L"out_implicit", &out_implicit::get, &out_implicit::set);
        property<GraphicsAdapter>::add_property(L"in_style", &in_style::get, &in_style::set);
        property<GraphicsAdapter>::add_property(L"in_label", &in_label::get, &in_label::set);
        property<GraphicsAdapter>::add_property(L"out_label", &out_label::get, &out_label::set);
        property<GraphicsAdapter>::add_property(L"style", &style::get, &style::set);
    }
}

GraphicsAdapter::~GraphicsAdapter()
{
}

bool GraphicsAdapter::toString(std::wostringstream& ostr)
{
    ostr << L"GraphicsAdapter.hxx: Dunno what to display there" << std::endl;
    return true;
}

std::wstring GraphicsAdapter::getTypeStr()
{
    return getSharedTypeStr();
}

std::wstring GraphicsAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */
