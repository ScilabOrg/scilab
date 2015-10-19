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

#ifndef __SLINT_BREAKS_IN_LOOP_CHECKER_HXX__
#define __SLINT_BREAKS_IN_LOOP_CHECKER_HXX__

#include <stack>
#include <tuple>

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Count the breaks or continues in a loop
 */
class BreaksInLoopChecker : public SLintChecker
{

    std::stack<std::tuple<const ast::Exp *, int, int>> stack;
    const int maxBreaks;
    const int maxContinues;

public:

    BreaksInLoopChecker(const std::wstring & checkerId, const int _maxBreaks, const int _maxContinues) : SLintChecker(checkerId), maxBreaks(_maxBreaks), maxContinues(_maxContinues) { }
    ~BreaksInLoopChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::FOREXP, ast::Exp::WHILEEXP, ast::Exp::BREAKEXP, ast::Exp::CONTINUEEXP };
    }

};

} // namespace slint

#endif // __SLINT_BREAKS_IN_LOOP_CHECKER_HXX__
