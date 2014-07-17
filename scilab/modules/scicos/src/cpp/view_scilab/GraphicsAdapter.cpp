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

        double* geom;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, &len, &geom);

        data[0] = geom[0];
        data[1] = geom[1];

        delete[] geom;
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
        double* geom;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, &len, &geom);

        geom[0] = current->get(0);
        geom[1] = current->get(1);

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, len, geom);
        delete[] geom;
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

        double* geom;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, &len, &geom);

        data[0] = geom[2];
        data[1] = geom[3];
        delete[] geom;
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
        double* geom;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, &len, &geom);

        geom[2] = current->get(0);
        geom[3] = current->get(1);

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), GEOMETRY, len, geom);
        delete[] geom;
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

        double* angle;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, &len, &angle);

        data[0] = angle[0];
        delete[] angle;
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
        double* angle;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, &len, &angle);

        angle[0] = (double) current->get(0);

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, len, angle);
        delete[] angle;
        return true;
    }
};

struct theta
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        int* data;
        types::Bool* o = new types::Bool(1, 1, &data);
        model::Block* adaptee = adaptor.getAdaptee();

        double* angle;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, &len, &angle);

        data[0] = angle[1];
        delete[] angle;
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
        double* angle;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, &len, &angle);

        angle[1] = (double) current->get(0);

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), ANGLE, len, angle);
        delete[] angle;
        return true;
    }
};

struct exprs
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        model::Block* adaptee = adaptor.getAdaptee();

        std::string* exprs;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, &len, &exprs);

        char* data;
        types::String* o = new types::String(len, 1);

        for (int i = 0; i < len; ++i)
        {
            o->set(i, exprs[i].data());
        }
        delete[] exprs;
        return o;
    }

    static bool set(GraphicsAdapter& adaptor, types::InternalType* v)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();

        model::Block* adaptee = adaptor.getAdaptee();
        std::string* exprs;
        size_t len;
        Controller::get_instance()->getObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, &len, &exprs);

        if (current->getRows() == (int) len && current->getCols() != 1)
        {
            return false;
        }
        if (current->getRows() == 1 && current->getCols() != (int) len)
        {
            return false;
        }
        for (int i = 0; i < len; ++i)
        {
            exprs[i].assign((char*) current->get(i));
        }

        Controller::get_instance()->setObjectProperty(adaptee->id(), adaptee->kind(), EXPRS, len, exprs);
        delete[] exprs;
        return true;
    }
};

struct pin
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

struct pout
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

struct pein
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

struct peout
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
