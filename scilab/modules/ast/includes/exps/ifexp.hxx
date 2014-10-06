/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_IFEXP_HXX
#define AST_IFEXP_HXX

#include <assert.h>
#include "controlexp.hxx"

namespace ast
{
/*
** \brief Abstract an If Expression node.
**
** \b Example: if (2097 = 0) then disp("fail") else disp("OK")
*/
class IfExp : public ControlExp
{
public:
    /*
    ** \brief Construct an If Expression node.
    ** \param location scanner position informations
    ** \param test chose the instruction to execute
    ** \param then instruction if test is true
    ** \param else instruction if test is false
    */
    IfExp(const Location& location,
          Exp& test, Exp& t, Exp& e)
        : ControlExp (location),
          _hasElse (true)
    {
        test.setParent(this);
        t.setParent(this);
        e.setParent(this);
        _exps[0] = &test;
        _exps[1] = &t;
        _exps[2] = &e;
    }

    IfExp(const Location& location,
          Exp& test, Exp& t)
        : ControlExp (location),
          _hasElse (false)
    {
        test.setParent(this);
        t.setParent(this);

        _exps[0] = &test;
        _exps[1] = &t;
        _exps[2] = new ast::CommentExp(location, new std::wstring(L"No else !!"));
        _exps[2]->setParent(this);
    }

    virtual ~IfExp()
    {
        delete _exps[0];
        delete _exps[1];
        delete _exps[2];
    }

    virtual IfExp* clone()
    {
        IfExp* cloned = new IfExp(getLocation(), *getTest().clone(), *getThen().clone(), *getElse().clone());
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    // \brief Visitors entry point.
public:
    virtual void	accept(Visitor& v)
    {
        v.visit (*this);
    }
    virtual void	accept(ConstVisitor& v) const
    {
        v.visit (*this);
    }

    // \brief Accessors.
public:
    // \brief Return the select condition of the loop (read only).
    const Exp&	getTest() const
    {
        return *_exps[0];
    }
    // \brief Return the select condition of the loop (read and write).
    Exp& getTest()
    {
        return *_exps[0];
    }

    // \brief Return the intructions if test is true (read only).
    const Exp&	getThen() const
    {
        return *_exps[1];
    }
    // \brief Return the instructions if test is true (read and write).
    Exp& getThen()
    {
        return *_exps[1];
    }

    // \brief Return the instruction if test is false (read only).
    const Exp& getElse() const
    {
        return *_exps[2];
    }
    // \brief Return the instruction if test is false (read and write).
    Exp& getElse()
    {
        return *_exps[2];
    }

    // \brief Return if there is an else body
    bool hasElse()
    {
        return _hasElse;
    }
    // \brief Return if there is an else body
    bool hasElse() const
    {
        return _hasElse;
    }

    virtual ExpType getType()
    {
        return IFEXP;
    }
    inline bool isIfExp() const
    {
        return true;
    }
protected:
    // \brief "has a value" qualifier.
    bool _hasElse;
};

} // namespace ast

#endif // !AST_IFEXP_HXX
