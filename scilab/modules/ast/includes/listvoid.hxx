/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef AST_LISTVOID_HXX
#define AST_LISTVOID_HXX

#include "var.hxx"

namespace ast
{
class ListVoid : public Var
{
public:

    ListVoid (const Location& location)
        : Var (location)
    {
    }

    ~ListVoid ()
    {
    }

    virtual ListVoid* clone()
    {
        Location* newloc = const_cast<Location*>(&location_get())->clone();
        ListVoid* cloned = new ListVoid(*newloc);
        cloned->set_verbose(is_verbose());
        return cloned;
    }

public:
    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }

    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }
};

} // namespace ast

#endif // !AST_LISTVOID_HXX
