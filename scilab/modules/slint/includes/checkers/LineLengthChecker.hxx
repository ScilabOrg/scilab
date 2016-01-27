/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

#ifndef __SLINT_LINE_LENGTH_CHECKER_HXX__
#define __SLINT_LINE_LENGTH_CHECKER_HXX__

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Check for maximal line length
 */
class LineLengthChecker : public SLintChecker
{

    const int max;

public:

    LineLengthChecker(const std::wstring & checkerId, const int _max) : SLintChecker(checkerId), max(_max) { }
    ~LineLengthChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::FUNCTIONDEC };
    }
};

} // namespace slint

#endif // __SLINT_LINE_LENGTH_CHECKER_HXX__
