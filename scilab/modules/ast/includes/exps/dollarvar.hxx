/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef AST_DOLLARVAR_HXX
#define AST_DOLLARVAR_HXX

#include "var.hxx"

namespace ast
{

/** \brief Abstract a Dollar Variable node.
**
** \b Example: $ */
class DollarVar : public Var
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a Dollar Variable node.
    ** \param location scanner position informations
    ** \param name the name of the variable
    */
    DollarVar (const Location& location)
        : Var (location)
    {
    }
    /** \brief Destroy a Field Variable node.
    **
    ** Delete name, see constructor. */
    ~DollarVar ()
    {
    }

    virtual DollarVar* clone()
    {
        DollarVar* cloned = new DollarVar(getLocation());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return e.getType() == DOLLARVAR;
    }

    /** \brief Accept a const visitor \a v. */
    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }
    /** \brief Accept a non-const visitor \a v. */
    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }

    virtual ExpType getType() const
    {
        return DOLLARVAR;
    }
    inline bool isDollarVar() const
    {
        return true;
    }
};

} // namespace ast

#endif // !AST_DOLLARVAR_HXX
