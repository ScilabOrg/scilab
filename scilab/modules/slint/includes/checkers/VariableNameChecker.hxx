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

#ifndef __SLINT_VARIABLE_NAME_CHECKER_HXX__
#define __SLINT_VARIABLE_NAME_CHECKER_HXX__

#include "SLintChecker.hxx"
#include "PCREMatcher.hxx"

namespace slint
{

class VariableNameChecker : public SLintChecker
{

    PCREMatcher matcher;
    int minimalLen;
    int maximalLen;

public:

    VariableNameChecker(const std::wstring & pattern, int _minimalLen, int _maximalLen) : matcher(pattern), minimalLen(_minimalLen), maximalLen(_maximalLen) { }
    ~VariableNameChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
	{
	    return { ast::Exp::SIMPLEVAR, ast::Exp::VARDEC };
	}
};

} // namespace slint

#endif // __SLINT_VARIABLE_NAME_CHECKER_HXX__
