/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_DGEMMEXP_HXX
#define AST_DGEMMEXP_HXX

#include "optimizedexp.hxx"
#include "internal.hxx"

namespace ast
{
class DGEMMExp : public OptimizedExp
{
public:
    DGEMMExp (const Location& location, Exp& a, bool transa, Exp& b, bool transb)
        : OptimizedExp (location), transa(transa), transb(transb)
    {
        _exps.push_back(&a);
        _exps.push_back(&b);

        a.setParent(this);
        b.setParent(this);
    }

    virtual ~DGEMMExp ()
    {
        //all children are ref so let original parent to destroy then
        _exps.clear();
    }

    virtual DGEMMExp* clone()
    {
        DGEMMExp* cloned = new DGEMMExp(getLocation(), *getA().clone(), transa, *getB().clone(), transb);
        return cloned;
    }

    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }

    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }

    const Exp& getA() const
    {
        return *_exps[0];
    }

    Exp& getA()
    {
        return *_exps[0];
    }

    const Exp& getB() const
    {
        return *_exps[1];
    }

    Exp& getB()
    {
        return *_exps[1];
    }

    bool isTransA()
    {
        return transa;
    }

    bool isTransA() const
    {
        return transa;
    }

    bool isTransB()
    {
        return transb;
    }

    bool isTransB() const
    {
        return transb;
    }

private:
    bool transa;
    bool transb;
};
} // namespace ast
#endif /* !AST_DGEMMEXP_HXX */
