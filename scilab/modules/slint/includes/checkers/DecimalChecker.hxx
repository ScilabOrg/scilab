/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LINT_DECIMAL_CHECKER_HXX__
#define __LINT_DECIMAL_CHECKER_HXX__

#include "SLintChecker.hxx"

namespace slint
{

class DecimalChecker : public SLintChecker
{

    const wchar_t character;
    const bool checkDot;

public:

    DecimalChecker(const wchar_t _character) : character(_character), checkDot(false) { }
    DecimalChecker(const bool _checkDot) : character(L'\0'), checkDot(_checkDot) { }
    DecimalChecker(const wchar_t _character, const bool _checkDot) : character(_character), checkDot(_checkDot) { }

    ~DecimalChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
	{
	    return { ast::Exp::DOUBLEEXP };
	}

};

} // namespace slint

#endif // __LINT_DECIMAL_CHECKER_HXX__
