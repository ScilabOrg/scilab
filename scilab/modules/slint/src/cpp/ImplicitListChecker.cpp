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

#include "core_math.h"

#include "checkers/ImplicitListChecker.hxx"

namespace slint
{

void ImplicitListChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::ListExp & le = static_cast<const ast::ListExp &>(e);
    if (le.getStart().isDoubleExp() && le.getStep().isDoubleExp() && le.getEnd().isDoubleExp())
    {
        const double start = static_cast<const ast::DoubleExp &>(le.getStart()).getValue();
        const double step = static_cast<const ast::DoubleExp &>(le.getStep()).getValue();
        const double end = static_cast<const ast::DoubleExp &>(le.getEnd()).getValue();
        if (ISNAN(start) || ISNAN(step) || ISNAN(end) || !finite(start) || !finite(step) || !finite(end))
        {
            result.report(context, e.getLocation(), *this, _("Invalid list, it contains NaN or Inf."));
        }
        else
        {
            if (start == end)
            {
                result.report(context, e.getLocation(), *this, _("List has the same start and end."));
            }
            if (step == 0)
            {
                result.report(context, e.getLocation(), *this, _("List has a null step."));
            }
            if ((start > end && step > 0) || (start < end && step < 0))
            {
                result.report(context, e.getLocation(), *this, _("List is empty."));
            }
        }
    }
    else if (le.getStart().isListExp() || le.getStep().isListExp() || le.getEnd().isListExp())
    {
        result.report(context, e.getLocation(), *this, _("Bad use of ':' operator."));
    }
}

void ImplicitListChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string ImplicitListChecker::getName() const
{
    return "ImplicitListChecker";
}
}
