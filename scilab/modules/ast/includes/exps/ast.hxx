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

/**
** \file ast.hxx
** Define the Abstract Syntax Tree mother class.
*/

#ifndef AST_AST_HXX
#define AST_AST_HXX

#include "location.hxx"
#include "visitor.hxx"

namespace ast
{

/** \brief Abstract an Abstract Syntax Tree node. */
class Ast
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct an Ast node.
    ** \param location scanner position informations */
    Ast (const Location& location) : _location (location)
    {
        nodeNumber = globalNodeNumber++;
    }


    /** \brief Destroys an Ast node. */
    virtual ~Ast ()
    {
    }
    /** \} */

    virtual Ast* clone() = 0;

    /** \name Accessors.
    ** \{ */
public:
    /** \brief Set scanner position informations.
    ** \param location scanner position informations
    **
    ** Return a reference to this.
    */
    Ast& location_set (const Location& location)
    {
        _location = location;
        return *this;
    }
    /** \brief Get scanner position informations stored. */
    inline const Location& location_get () const
    {
        return _location;
    }

    /** \brief Get scanner position informations stored. */
    inline Location& location_get ()
    {
        return _location;
    }
    /** \} */

    /** \name Visitors entry point.
    ** \{ */
public:
    /** \brief Accept a const visitor \a v. */
    virtual void accept (ConstVisitor& v) const = 0;
    /** \brief Accept a non const visitor \a v. */
    virtual void accept (Visitor& v) = 0;
    /** \} */

    void elapsedtime_set(double _dblElapsedTime)
    {
        m_dblElapsedTime = _dblElapsedTime;
    }

    size_t nodeNumber_get() const
    {
        return nodeNumber;
    }

    void nodeNumber_set(size_t _nodeNumber)
    {
        nodeNumber = _nodeNumber;
    }

private:
    /** \brief Construct an Ast by copy. */
    Ast (const Ast&);
    /** \brief Assign an Ast to this. */
    Ast& operator= (const Ast&);

    /** \brief Scanner position informations. */
    Location _location;
    double m_dblElapsedTime;
    size_t nodeNumber;
    static EXTERN_AST size_t globalNodeNumber;
};

} // namespace ast

#endif // !AST_AST_HXX
