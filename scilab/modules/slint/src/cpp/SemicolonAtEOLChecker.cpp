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

#include "checkers/SemicolonAtEOLChecker.hxx"

namespace slint
{
void SemicolonAtEOLChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isSeqExp())
    {
        const ast::exps_t & exps = static_cast<const ast::SeqExp &>(e).getExps();
        if (!exps.empty())
        {
            int prevline = exps.front()->getLocation().last_line;
            ast::Exp * prevexp = exps.front();
            for (ast::exps_t::const_iterator i = std::next(exps.begin()), end = exps.end(); i != end; ++i)
            {
                if ((*i)->getLocation().first_line != prevline)
                {
                    check(prevexp, context, result);
                }
                prevexp = *i;
                prevline = prevexp->getLocation().last_line;
            }
            check(prevexp, context, result);
        }
    }
}

void SemicolonAtEOLChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::wstring SemicolonAtEOLChecker::getName() const
{
    return L"SemicolonAtEOLChecker";
}

void SemicolonAtEOLChecker::check(const ast::Exp * e, SLintContext & context, SLintResult & result) const
{
    ast::Exp::ExpType ty = e->getType();
    if (ty != ast::Exp::IFEXP && ty != ast::Exp::WHILEEXP && ty != ast::Exp::TRYCATCHEXP && ty != ast::Exp::FOREXP && ty != ast::Exp::FUNCTIONDEC && ty != ast::Exp::COMMENTEXP && ty != ast::Exp::SELECTEXP && ty != ast::Exp::SEQEXP)
    {
        std::pair<unsigned int, unsigned int> out;
        if (context.getPosition(e->getLocation(), out) && out.first < out.second)
        {
            if (!matcher.match(context.getCode() + out.first, out.second - out.first, false))
            {
                result.report(context, e->getLocation(), L"Instruction not finished by a semi-colon.");
            }
        }
    }
}
}
