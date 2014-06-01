/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
** \file functiondec.hxx
** Define the Function Declaration class.
*/

#ifndef AST_FUNCTIONDEC_HXX
#define AST_FUNCTIONDEC_HXX

#include <list>

#include "dec.hxx"
#include "symbol.hxx"
#include "exp.hxx"
#include "arraylistvar.hxx"

using namespace std;

namespace ast
{
/*
** \brief Abstract a Function Declaration node.
**
** \b Example: function foo(s : string) : int = (print(s); 2097)
*/
class FunctionDec : public Dec
{
    // \name Ctor & dtor.
public:
    /*
    ** \brief Construct a Function Declaration node.
    ** \param location scanner position informations
    ** \param name of function
    ** \param list of params
    ** \param list of returns
    ** \param body
    */
    FunctionDec (const Location& location,
                 symbol::Symbol& name,
                 ArrayListVar& args,
                 ArrayListVar& returns,
                 Exp& body)
        : Dec (location),
          _name (name),
          _args (&args),
          _returns (&returns),
          _body (&body),
          _stack(NULL)
    {
    }

    virtual ~FunctionDec ()
    {
        delete _body;
        delete _args;
        delete _returns;
        delete &_name;
    }

    virtual FunctionDec* clone()
    {
        Location* newloc = const_cast<Location*>(&location_get())->clone();
        FunctionDec* cloned = new FunctionDec(*newloc, * new symbol::Symbol(name_get()), *args_get().clone(), *returns_get().clone(), *body_get().clone());
        cloned->set_verbose(is_verbose());
        return cloned;
    }

    // \name Visitors entry point.
public:
    // \brief Accept a const visitor
    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }
    // \brief Accept a non-const visitor
    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }


    // \name Accessors.
public:
    symbol::Symbol& name_get (void) const
    {
        return _name;
    }

    const Exp& body_get (void) const
    {
        return *_body;
    }

    Exp& body_get (void)
    {
        return *_body;
    }

    const ArrayListVar& args_get () const
    {
        return *_args;
    }

    ArrayListVar& args_get ()
    {
        return *_args;
    }

    const ArrayListVar& returns_get () const
    {
        return *_returns;
    }

    ArrayListVar& returns_get ()
    {
        return *_returns;
    }

    void body_set(Exp *body)
    {
        _body = body;
    }

    symbol::Variable* stack_get()
    {
        if (_stack == NULL)
        {
            _stack = symbol::Context::getInstance()->getOrCreate(_name);
        }

        return _stack;
    }

protected:
    symbol::Symbol&    _name;
    symbol::Variable*    _stack;
    ArrayListVar*	_args;
    ArrayListVar*	_returns;
    Exp*		_body;
};

} // namespace ast

#endif // !AST_FUNCTIONDEC_HXX
