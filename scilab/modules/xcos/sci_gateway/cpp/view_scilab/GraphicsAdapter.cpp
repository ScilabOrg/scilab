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

#include "double.hxx"

#include "GraphicsAdapter.hxx"

namespace org_scilab_modules_xcos
{
namespace view_scilab
{

struct orig
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        model::Geometry g = adaptor.get_adaptee()->get_geometry();
        data[0] = g.x;
        data[1] = g.y;
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

        model::Geometry g = adaptor.get_adaptee()->get_geometry();
        g.x = current->get(0);
        g.y = current->get(1);
        return true;
    }
};

struct sz
{

    static types::InternalType* get(const GraphicsAdapter& adaptor)
    {
        double* data;
        types::Double* o = new types::Double(1, 2, &data);
        model::Geometry g = adaptor.get_adaptee()->get_geometry();
        data[0] = g.width;
        data[1] = g.height;
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

        model::Geometry g = adaptor.get_adaptee()->get_geometry();
        g.width = current->get(0);
        g.height = current->get(1);
        return true;
    }
};

struct flip
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

struct theta
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

struct exprs
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


GraphicsAdapter::GraphicsAdapter(const GraphicsAdapter& o) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_xcos::model::Block>(o) { }

GraphicsAdapter::GraphicsAdapter(org_scilab_modules_xcos::model::Block* o) :
    BaseAdapter<GraphicsAdapter, org_scilab_modules_xcos::model::Block>(o)
{
    if (properties_has_not_been_set())
    {
        add_property(L"orig", &orig::get, &orig::set);
        add_property(L"sz", &sz::get, &sz::set);
        add_property(L"flip", &flip::get, &flip::set);
        add_property(L"theta", &theta::get, &theta::set);
        add_property(L"exprs", &exprs::get, &exprs::set);
        add_property(L"pin", &pin::get, &pin::set);
        add_property(L"pout", &pout::get, &pout::set);
        add_property(L"pein", &pein::get, &pein::set);
        add_property(L"peout", &peout::get, &peout::set);
        add_property(L"in_implicit", &in_implicit::get, &in_implicit::set);
        add_property(L"out_implicit", &out_implicit::get, &out_implicit::set);
        add_property(L"in_style", &in_style::get, &in_style::set);
        add_property(L"in_label", &in_label::get, &in_label::set);
        add_property(L"out_label", &out_label::get, &out_label::set);
        add_property(L"style", &style::get, &style::set);
    }
}

GraphicsAdapter::~GraphicsAdapter()
{
}

} /* view_scilab */
} /* namespace org_scilab_modules_xcos */
