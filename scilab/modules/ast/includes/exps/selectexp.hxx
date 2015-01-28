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
              exps_t& cases,
              Exp& defaultCase)
        : ControlExp (location),
          _hasDefault(true)
    {
        select.setParent(this);
        _exps.push_back(&select);

        for (exps_t::const_iterator it = cases.begin(), itEnd = cases.end(); it != itEnd ; ++it)
        {
            (*it)->setParent(this);
            _exps.push_back(*it);
        }

        if (&defaultCase != NULL)
        {
            defaultCase.setParent(this);
            _exps.push_back(&defaultCase);
        }
        else
        {
            _hasDefault = false;
        }
    }

    SelectExp(const Location& location,
              Exp& select,
              exps_t& cases)
        : ControlExp (location),
          _hasDefault(false)
    {
        select.setParent(this);
        _exps.push_back(&select);

        for (exps_t::const_iterator it = cases.begin(), itEnd = cases.end(); it != itEnd ; ++it)
        {
            (*it)->setParent(this);
            _exps.push_back(*it);
        }
    }

    ~SelectExp()
    {
    }

    virtual SelectExp* clone()
    {
        exps_t cases;
        exps_t::const_iterator it = ++(_exps.begin());
        exps_t::const_iterator itEnd = _exps.end();
        if (_hasDefault)
        {
            --itEnd;
        }

        for (; it != itEnd ; ++it)
        {
            cases.push_back((*it)->clone());
        }

        SelectExp* cloned = NULL;
        if (_hasDefault)
        {
            cloned = new SelectExp(getLocation(), *getSelect()->clone(), cases, *getDefaultCase()->clone());
        }
        else
        {
            cloned = new SelectExp(getLocation(), *getSelect()->clone(), cases);
        }

        cloned->setVerbose(isVerbose());
        return cloned;
    }

public :
    Exp* getSelect() const
    {
        return _exps[0];
    }

    exps_t* getCases() const
    {
        exps_t* cases = new exps_t;
        exps_t::const_iterator it = ++(_exps.begin());
        exps_t::const_iterator itEnd = _exps.end();
        if (_hasDefault)
        {
            --itEnd;
        }

        for (; it != const_cast<const exps_t::const_iterator>(itEnd) ; ++it)
        {
            cases->push_back(*it);
        }

        return cases;
    }

    Exp* getDefaultCase() const
    {
        if (_hasDefault)
        {
            return _exps.back();
        }
        else
        {
            return NULL;
        }
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

    virtual ExpType getType()
    {
        return SELECTEXP;
    }

    inline bool isSelectExp() const
    {
        return true;
    }

    bool hasDefault()
    {
        return _hasDefault;
    }

    bool hasDefault() const
    {
        return _hasDefault;
    }

    bool _hasDefault;
};

}


#endif /* !__AST_SELECT_EXP_HXX__ */
