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

#ifndef AST_DOUBLEEXP_HXX
#define AST_DOUBLEEXP_HXX

#include "constexp.hxx"

#include "visitor.hxx"

#include "double.hxx"

namespace ast
{
/** \brief Abstract an Double Expression node.
**
** \b Example:  42.51 */
class DoubleExp : public ConstExp
{
public:
    DoubleExp (const Location& location, double value)
        : ConstExp (location),
          _value (value),
          _bigDouble (NULL)
    {
    }
    /** \brief Destroy an Double Expression node.
    **
    ** Delete size et init (exp) (see constructor). */
    virtual ~DoubleExp ()
    {
        if (_bigDouble)
        {
            _bigDouble->DecreaseRef();
            _bigDouble->killMe();
        }
    }
    /** \} */

    virtual DoubleExp* clone()
    {
        DoubleExp* cloned = new DoubleExp(location_get(), value_get());
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
    /** \brief Return the value */
    double value_get() const
    {
        return _value;
    }
    /** \} */

    types::Double* getBigDouble() const
    {
        return _bigDouble;
    }

    void setBigDouble(types::Double *pdbl)
    {
        if (_bigDouble)
        {
            _bigDouble->DecreaseRef();
            _bigDouble->killMe();
        }

        _bigDouble = pdbl;
        _bigDouble->IncreaseRef();
    }

    inline bool is_double_exp() const
	{
	    return true;
	}
    
    inline DoubleExp * neg()
	{
	    _value = -_value;
	    return this;
	}
    
protected:
    double     _value;
    types::Double*    _bigDouble;
};

} // namespace ast
#endif
