/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CONSTANT_VALUE_HXX__
#define __CONSTANT_VALUE_HXX__

#include <complex>
#include <iostream>
#include <string>
#include <type_traits>

#include "gvn/GVN.hxx"

namespace types
{
class InternalType;
}

namespace analysis
{
class ConstantValue
{

public:

    enum Kind : unsigned char { GVNVAL, ITVAL, UNKNOWN };

private:

    union Value
    {
        GVN::Value * gvnVal;
        types::InternalType * pIT;

        Value() { }
        Value(GVN::Value * val) : gvnVal(val) { }
        Value(types::InternalType * val) : pIT(val) { }
    };

    Value val;
    Kind kind;

public:

    ConstantValue() : kind(UNKNOWN) { }
    ConstantValue(GVN::Value * _val) : val(_val), kind(GVNVAL) { }
    ConstantValue(types::InternalType * _val);
    ConstantValue(const ConstantValue & cv);
    ConstantValue(ConstantValue && cv) : val(cv.val), kind(cv.kind)
    {
        cv.kind = UNKNOWN;
    }

    ConstantValue & operator=(const ConstantValue & R);
    ConstantValue & operator=(types::InternalType * const pIT);
    ConstantValue & operator=(GVN::Value * const _val);
    ConstantValue & operator=(ConstantValue && R);

    ~ConstantValue();

    inline Kind getKind() const
    {
        return kind;
    }

    inline bool isKnown() const
    {
        return kind != UNKNOWN;
    }

    inline types::InternalType * getIT() const
    {
        if (kind == ITVAL)
        {
            return val.pIT;
        }

        return nullptr;
    }

    inline GVN::Value * getGVNValue() const
    {
        if (kind == GVNVAL)
        {
            return val.gvnVal;
        }

        return nullptr;
    }

    bool getGVNValue(GVN & gvn, GVN::Value *& _val) const;
    bool getDblValue(double & _val) const;
    bool getBoolValue(bool & _val) const;
    bool getCplxValue(std::complex<double> & _val) const;
    bool getStrValue(std::wstring & val) const;

    template<typename T> inline T get() const;

    void merge(const ConstantValue & cv);

    friend std::wostream & operator<<(std::wostream & out, const ConstantValue & cv);
};

template<> inline GVN::Value * ConstantValue::get<>() const
{
    return val.gvnVal;
}
template<> inline types::InternalType * ConstantValue::get<>() const
{
    return val.pIT;
}

} // namespace analysis

#endif // __CONSTANT_VALUE_HXX__
