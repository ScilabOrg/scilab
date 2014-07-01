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

#ifndef BASEADAPTER_HXX_
#define BASEADAPTER_HXX_

#include <algorithm>
#include <string>
#include <utility>

#include "types.hxx"
#include "user.hxx"

#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

/**
 * Base definition of the adapter pattern, implement the get / set dispatch.
 *
 * Note that sub-classes are responsible to fill the fields accordingly to theirs interfaces.
 */
template<class Adaptor, class Adaptee>
class BaseAdapter : public types::User<Adaptor>
{

public:
    BaseAdapter(Adaptee* o) : adaptee(o) {};
    BaseAdapter(const BaseAdapter& o) : adaptee(o.adaptee) {};
    ~BaseAdapter() {};

    typedef types::InternalType* (*getter_t)(const Adaptor& adaptor);
    typedef bool (*setter_t)(Adaptor& adaptor, types::InternalType* v);

    /**
     * A property used as a getter/setter for a specific name
     */
    struct property
    {
    public:
        property(std::wstring prop, getter_t g, setter_t s) : name(prop), get(g), set(s) {};
        ~property() {};

        std::wstring name;
        getter_t get;
        setter_t set;

        bool operator< (const std::wstring& v) const
        {
            return name < v;
        }
    };

    typedef std::vector<property> props_t;
    typedef typename props_t::iterator props_t_it;

    /**
     * @return the Adaptee instance
     */
    Adaptee* get_adaptee() const
    {
        return adaptee;
    };
    /**
     * @param the Adaptee instance to set
     */
    void set_adaptee(Adaptee* v)
    {
        adaptee = v;
    };

    /**
     * @return true if the properties has already been setup, false otherwise.
     */
    static bool properties_has_not_been_set()
    {
        return fields.empty();
    }

    /**
     * Add a property to an Adaptor
     */
    static void add_property(const std::wstring& name, getter_t g, setter_t s)
    {
        props_t_it pos = std::lower_bound(fields.begin(), fields.end(), name);
        fields.insert(pos, property(name, g, s));
    }

    bool has(const std::wstring& _sKey) const
    {
        props_t_it found = std::binary_search(fields.begin(), fields.end(), _sKey);
        return found != fields.end();
    }

    types::InternalType* get(const std::wstring& _sKey) const
    {
        props_t_it found = std::lower_bound(fields.begin(), fields.end(), _sKey);
        if (found != fields.end())
        {
            return found->get(static_cast<Adaptor*>(this));
        }
        return 0;
    }

    bool set(const std::wstring& _sKey, types::InternalType* v)
    {
        props_t_it found = std::lower_bound(fields.begin(), fields.end(), _sKey);
        if (found != fields.end())
        {
            return found->set(*static_cast<Adaptor*>(this), v);
        }
        return false;
    }

    /*
     * All following methods should be implemented by each template instance
     */

    bool toString(std::wostringstream& ostr);
    std::wstring getTypeStr();
    std::wstring getShortTypeStr();

private:
    static props_t fields;

    Adaptee* adaptee;
};

} /* view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* BASEADAPTER_HXX_ */
