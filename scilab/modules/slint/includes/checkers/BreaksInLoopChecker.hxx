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

#ifndef __BREAKS_IN_LOOP_CHECKER_HXX__
#define __BREAKS_IN_LOOP_CHECKER_HXX__

#include <stack>
#include <tuple>

#include "SLintChecker.hxx"

namespace slint
{

class BreaksInLoopChecker : public SLintChecker
{

    std::stack<std::tuple<const ast::Exp *, int, int>> stack;
    const unsigned int maxBreaks;
    const unsigned int maxContinues;

public:

    BreaksInLoopChecker(const int _maxBreaks, const int _maxContinues) : maxBreaks(_maxBreaks >= 0 ? _maxBreaks : 0), maxContinues(_maxContinues >= 0 ? _maxContinues : 0) { }
    ~BreaksInLoopChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::wstring getName() const;

};

} // namespace slint

#endif // __BREAKS_IN_LOOP_CHECKER_HXX__
