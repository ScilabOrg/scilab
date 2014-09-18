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
#include <iterator>
#include <algorithm>

#include "internal.hxx"
#include "list.hxx"
#include "types.hxx"
#include "user.hxx"
#include "double.hxx"

#include "Controller.hxx"
#include "LinkAdapter.hxx"
#include "model/Link.hxx"

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

const std::string split ("split");
const std::string lsplit ("lsplit");
const std::string limpsplit ("limpsplit");

struct xx
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        double* data;
        int size = (int)controlPoints.size() / 2;
        types::Double* o = new types::Double(size, 1, &data);

#ifdef _MSC_VER
        std::copy(controlPoints.begin(), controlPoints.begin() + size, stdext::checked_array_iterator<double*>(data, size));
#else
        std::copy(controlPoints.begin(), controlPoints.begin() + size, data);
#endif
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        int newXSize = current->getSize();
        int oldXSize = static_cast<int>(controlPoints.size() / 2);
        std::vector<double> newControlPoints (controlPoints);

        if (newXSize == oldXSize)
        {
#ifdef _MSC_VER
            std::copy(current->getReal(), current->getReal() + newXSize, stdext::checked_array_iterator<double*>( newControlPoints.begin(), newXSize ));
#else
            std::copy(current->getReal(), current->getReal() + newXSize, newControlPoints.begin());
#endif
        }
        else
        {
            newControlPoints.resize(2 * current->getSize(), 0);

#ifdef _MSC_VER
            std::copy(current->getReal(), current->getReal() + newXSize, stdext::checked_array_iterator<double*>( newControlPoints.begin(), newXSize ));
            std::copy(controlPoints.begin() + oldXSize, controlPoints.begin() + oldXSize + std::min(newXSize, oldXSize), stdext::checked_array_iterator<double*>( newControlPoints.begin() + newXSize, std::min(newXSize, oldXSize) ));
#else
            std::copy(current->getReal(), current->getReal() + newXSize, newControlPoints.begin());
            std::copy(controlPoints.begin() + oldXSize, controlPoints.begin() + oldXSize + std::min(newXSize, oldXSize), newControlPoints.begin() + newXSize);
#endif
        }

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, newControlPoints);
        return true;
    }
};

struct yy
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        double* data;
        int size = (int)controlPoints.size() / 2;
        types::Double* o = new types::Double(size, 1, &data);

#ifdef _MSC_VER
        std::copy(controlPoints.begin() + size, controlPoints.end(), stdext::checked_array_iterator<double*>(data, size));
#else
        std::copy(controlPoints.begin() + size, controlPoints.end(), data);
#endif
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();

        std::vector<double> controlPoints;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, controlPoints);

        int newYSize = current->getSize();
        int oldYSize = static_cast<int>(controlPoints.size() / 2);
        std::vector<double> newControlPoints (controlPoints);

        if (newYSize == oldYSize)
        {
#ifdef _MSC_VER
            std::copy(current->getReal(), current->getReal() + newYSize, stdext::checked_array_iterator<double*>( newControlPoints.begin() + newYSize, newYSize ));
#else
            std::copy(current->getReal(), current->getReal() + newYSize, newControlPoints.begin() + newYSize);
#endif
        }
        else
        {
            newControlPoints.resize(2 * current->getSize());

#ifdef _MSC_VER
            std::copy(current->getReal(), current->getReal() + newYSize, stdext::checked_array_iterator<double*>( newControlPoints.begin() + newYSize, newYSize ));
#else
            std::copy(current->getReal(), current->getReal() + newYSize, newControlPoints.begin() + newYSize);
#endif
            if (newYSize > oldYSize)
            {
                std::fill(newControlPoints.begin() + oldYSize, newControlPoints.begin() + oldYSize + newYSize, 0);
            }
        }

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), CONTROL_POINTS, newControlPoints);
        return true;
    }
};

struct id
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::string id;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), LABEL, id);

        types::String* o = new types::String(1, 1);
        o->set(0, id.data());

        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        if (v->getType() != types::InternalType::ScilabString)
        {
            return false;
        }

        types::String* current = v->getAs<types::String>();
        if (current->getSize() != 1)
        {
            return false;
        }

        model::Link* adaptee = adaptor.getAdaptee();

        std::string id;
        char* c_str = wide_string_to_UTF8(current->get(0));
        id = std::string(c_str);
        FREE(c_str);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), LABEL, id);
        return true;
    }
};

struct thick
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        std::vector<double> thick;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), THICK, thick);

        double* data;
        types::Double* o = new types::Double(1, 2, &data);

        data[0] = thick[0];
        data[1] = thick[1];
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }

        std::vector<double> thick (2);
        thick[0] = current->get(0);
        thick[1] = current->get(1);

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), THICK, thick);
        return true;
    }
};

struct ct
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        int color;
        int kind;
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), COLOR, color);
        controller.getObjectProperty(adaptee->id(), adaptee->kind(), KIND, kind);

        double* data;
        types::Double* o = new types::Double(1, 2, &data);

        data[0] = static_cast<double>(color);
        data[1] = static_cast<double>(kind);
        return o;
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        model::Link* adaptee = adaptor.getAdaptee();

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        types::Double* current = v->getAs<types::Double>();
        if (current->getRows() != 1 || current->getCols() != 2)
        {
            return false;
        }
        if (floor(current->get(0)) != current->get(0) || floor(current->get(1)) != current->get(1))
        {
            return false;
        }

        int color = static_cast<int>(current->get(0));
        int kind  = static_cast<int>(current->get(1));

        controller.setObjectProperty(adaptee->id(), adaptee->kind(), COLOR, color);
        controller.setObjectProperty(adaptee->id(), adaptee->kind(), KIND, kind);
        return true;
    }
};

struct from
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        return adaptor.getFrom();
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        adaptor.setFrom(v->clone());
        return true;
    }
};

struct to
{

    static types::InternalType* get(const LinkAdapter& adaptor, const Controller& controller)
    {
        return adaptor.getTo();
    }

    static bool set(LinkAdapter& adaptor, types::InternalType* v, Controller& controller)
    {
        // silent unused parameter warnings
        (void) controller;

        if (v->getType() != types::InternalType::ScilabDouble)
        {
            return false;
        }

        adaptor.setTo(v->clone());
        return true;
    }
};

} /* namespace */

template<> property<LinkAdapter>::props_t property<LinkAdapter>::fields = property<LinkAdapter>::props_t();

LinkAdapter::LinkAdapter(bool ownAdaptee, org_scilab_modules_scicos::model::Link* adaptee) :
    BaseAdapter<LinkAdapter, org_scilab_modules_scicos::model::Link>(ownAdaptee, adaptee)
{
    if (property<LinkAdapter>::properties_have_not_been_set())
    {
        property<LinkAdapter>::fields.reserve(7);
        property<LinkAdapter>::add_property(L"xx", &xx::get, &xx::set);
        property<LinkAdapter>::add_property(L"yy", &yy::get, &yy::set);
        property<LinkAdapter>::add_property(L"id", &id::get, &id::set);
        property<LinkAdapter>::add_property(L"thick", &thick::get, &thick::set);
        property<LinkAdapter>::add_property(L"ct", &ct::get, &ct::set);
        property<LinkAdapter>::add_property(L"from", &from::get, &from::set);
        property<LinkAdapter>::add_property(L"to", &to::get, &to::set);
    }

    double* dataFrom;
    From = new types::Double(1, 3, &dataFrom);
    double* dataTo;
    To = new types::Double(1, 3, &dataTo);

    dataFrom[0] = 0;
    dataFrom[1] = 0;
    dataFrom[2] = 0;
    dataTo[0]   = 0;
    dataTo[1]   = 0;
    dataTo[2]   = 0;
}

LinkAdapter::~LinkAdapter()
{
    delete From;
    delete To;
}

std::wstring LinkAdapter::getTypeStr()
{
    return getSharedTypeStr();
}
std::wstring LinkAdapter::getShortTypeStr()
{
    return getSharedTypeStr();
}

types::InternalType* LinkAdapter::getFrom() const
{
    return From->clone();
}

void LinkAdapter::setFrom(types::InternalType* v)
{
    delete From;
    From = v->clone();
}

types::InternalType* LinkAdapter::getTo() const
{
    return To->clone();
}

void LinkAdapter::setTo(types::InternalType* v)
{
    delete To;
    To = v->clone();
}

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */
