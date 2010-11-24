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
            const std::wstring name) 
            : Var (location),
            _name (name)
        {
        }
        /** \brief Destroy a Field Variable node.
        **
        ** Delete name, see constructor. */
        virtual ~SimpleVar ()
        {
        }

        virtual SimpleVar* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            return new SimpleVar(*newloc, name_get());
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
        const std::wstring& name_get () const
        {
            return _name;
        }
        /** \} */


        bool operator== (const SimpleVar &rhs) const
        {
            return _name == rhs.name_get();
        }

    protected:
        /** \brief Variable's name */
        std::wstring _name;
    };

} // namespace ast

#endif // !AST_SIMPLEVAR_HXX
