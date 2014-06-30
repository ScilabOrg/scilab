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


#ifndef AST_SIMPLEVAR_HXX
#define AST_SIMPLEVAR_HXX

#include "var.hxx"
#include "context.hxx"

namespace ast
{
/** \brief Abstract a Simple Variable node.
**
** \b Example: i */
class SimpleVar : public Var
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a Simple Variable node.
    ** \param location scanner position informations
    ** \param name the name of the variable
    */
    SimpleVar (const Location& location,
               symbol::Symbol& name)
        : Var (location),
          _name (name),
          _stack(NULL)
    {
    }
    /** \brief Destroy a Field Variable node.
    **
    ** Delete name, see constructor. */
    virtual ~SimpleVar ()
    {
        delete &_name;
    }

    virtual SimpleVar* clone()
    {
        SimpleVar* cloned = new SimpleVar(location_get(), *new symbol::Symbol(name_get().name_get()));
        cloned->set_verbose(is_verbose());
        return cloned;
    }

    /** \name Visitors entry point.
    ** \{ */
public:
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
    /** \} */


    /** \name Accessors.
    ** \{ */
public:
    /** \brief Return the Variable's name. */
    symbol::Symbol& name_get () const
    {
        return _name;
    }

    symbol::Variable* stack_get()
    {
        if (_stack == NULL)
        {
            _stack = symbol::Context::getInstance()->getOrCreate(_name);
        }

        return _stack;
    }
    /** \} */


    bool operator== (const SimpleVar &rhs) const
    {
        return _name == rhs.name_get();
    }

    inline bool is_simple_var() const
    {
        return true;
    }

protected:
    /** \brief Variable's name */
    symbol::Symbol& _name;
    symbol::Variable* _stack;
};

} // namespace ast

#endif // !AST_SIMPLEVAR_HXX
