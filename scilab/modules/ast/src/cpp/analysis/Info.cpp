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

#include "allexp.hxx"
#include "allvar.hxx"
#include "data/Info.hxx"

namespace analysis
{

void Info::merge(const Info & info, bool & isSameData)
{
    R = R || info.R;
    W = W || info.W;
    O = O || info.O;
    isint = isint && info.isint;
    if (local != info.local)
    {
        local = Local::INFO_UNKNOWN;
    }
    cleared = cleared && info.cleared;
    exists = exists || info.exists;
    constant.merge(info.constant);
    maxIndex.mergeAsMax(info.maxIndex);
    type.merge(info.type);
    isSameData = data->same(info.data);
    data->setValidity(isSameData);
    // No need to merge range since this info is just used in for loop
}

void Info::addData(const bool known, const symbol::Symbol & sym)
{
    data = new Data(known, sym);
}

void Info::addData(Data * _data, const symbol::Symbol & sym)
{
    data = _data;
    data->add(sym);
}

SymbolicRange & Info::getRange()
{
    return range;
}

const SymbolicRange & Info::getRange() const
{
    return range;
}

SymbolicDimension & Info::getMaxIndex()
{
    return maxIndex;
}

const SymbolicDimension & Info::getMaxIndex() const
{
    return maxIndex;
}

SymbolicRange & Info::setRange(SymbolicRange & _range)
{
    range = _range;
    return range;
}

ConstantValue & Info::getConstant()
{
    return constant;
}

const ConstantValue & Info::getConstant() const
{
    return constant;
}

ConstantValue & Info::setConstant(ConstantValue & val)
{
    constant = val;
    return constant;
}

bool Info::isknown() const
{
    return local == Local::INFO_TRUE;
}

const symbol::Symbol & Info::getRightSym(ast::Exp * exp)
{
    return static_cast<const ast::SimpleVar &>(static_cast<const ast::AssignExp *>(exp)->getRightExp()).getSymbol();
}

const TIType & Info::getType() const
{
    return type;
}

std::wostream & operator<<(std::wostream & out, const Info & info)
{
    out << L"Type: " << info.type << L" - RWO:"
        << (info.R ? L"T" : L"F")
        << (info.W ? L"T" : L"F")
        << (info.O ? L"T" : L"F")
        << L" - int:" << (info.isint ? L"T" : L"F")
        << L" - local:" << (info.local == Info::Local::INFO_TRUE ? L"T" : (info.local == Info::Local::INFO_FALSE ? L"F" : L"U"))
        << L" - cleared:" << (info.cleared ? L"T" : L"F")
        << L" - exists:" << (info.exists ? L"T" : L"F")
        << L" - constant:" << info.constant;

    out << L" - data:";
    if (info.data)
    {
        out << *info.data;
    }
    else
    {
        out << L"null";
    }

    return out;
}

} // namespace analysis
