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

#ifndef AST_STRINGEXP_HXX
#define AST_STRINGEXP_HXX

#include "constexp.hxx"
#include "string.hxx"

namespace ast
{
/** \brief Abstract an string Expression node.
**
** \b Example: string*/
class StringExp : public ConstExp
{
public:
    StringExp (const Location& location,
               std::wstring value)
        : ConstExp (location),
          _value (value),
          _bigString (NULL)
    {
    }
    /** \brief Destroy an string Exp node.
    **
    ** Delete value (see constructor). */
    virtual ~StringExp ()
    {
        if (_bigString)
        {
            _bigString->DecreaseRef();
            if (_bigString->isDeletable())
            {
                delete _bigString;
            }
        }
    }
    /** \} */

    virtual StringExp* clone()
    {
        Location* newloc = const_cast<Location*>(&location_get())->clone();
        StringExp* cloned = new StringExp(*newloc, value_get());
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

    types::String* getBigString() const
    {
        return _bigString;
    }


    void setBigString(types::String *_pS)
    {
        if (_bigString && _bigString->isRef())
        {
            _bigString->DecreaseRef();
            if (_bigString->isDeletable())
            {
                delete _bigString;
            }
        }
        _bigString = _pS;
        _bigString->IncreaseRef();
    }

    /** \name Accessors.
    ** \{ */
public:
    /** \brief Return the name of the type name (read only). */
    const std::wstring value_get () const
    {
        return _value;
    }
    /** \} */
protected:
    const std::wstring _value;
    types::String* _bigString;
};

} // namespace ast

#endif // !AST_STRINGEXP_HXX
