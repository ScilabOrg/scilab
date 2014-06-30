/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file selectexp.hxx
 ** Define Select Expression class
 */

#ifndef __AST_SELECT_EXP_HXX__
#define __AST_SELECT_EXP_HXX__

#include "controlexp.hxx"
#include "exp.hxx"
#include "caseexp.hxx"
#include "seqexp.hxx"

namespace ast
{
class SelectExp : public ControlExp
{
public :
    SelectExp(const Location& location,
              Exp& select,
              cases_t& cases,
              SeqExp& defaultCase)
        : ControlExp (location),
          _selectme (&select),
          _cases (&cases),
          _default (&defaultCase)
    {
    }

    SelectExp(const Location& location,
              Exp& select,
              cases_t& cases)
        : ControlExp (location),
          _selectme (&select),
          _cases (&cases),
          _default(NULL)
    {
    }

    ~SelectExp()
    {
        delete _selectme;

        for (cases_t::const_iterator it = _cases->begin(), itEnd = _cases->end(); it != itEnd ; ++it)
        {
            delete *it;
        }

        delete _cases;

        if (_default != NULL)
        {
            delete _default;
        }
    }

    virtual SelectExp* clone()
    {
        cases_t* cases = new cases_t;
        for (cases_t::const_iterator it = cases_get()->begin() ; it != cases_get()->end() ; it++)
        {
            cases->push_back((*it)->clone());
        }

        SelectExp* cloned = NULL;
        if (_default != NULL)
        {
            cloned = new SelectExp(location_get(), *select_get()->clone(), *cases, *default_case_get()->clone());
        }
        else
        {
            cloned = new SelectExp(location_get(), *select_get()->clone(), *cases);
        }

        cloned->set_verbose(is_verbose());
        return cloned;
    }

public :
    Exp* select_get() const
    {
        return _selectme;
    }
    cases_t* cases_get() const
    {
        return _cases;
    }
    SeqExp* default_case_get() const
    {
        return _default;
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

private :
    Exp* _selectme;
    cases_t* _cases;
    SeqExp* _default;
};

}


#endif /* !__AST_SELECT_EXP_HXX__ */
