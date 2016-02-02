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
 *
 */

#include "AnalysisVisitor.hxx"
#include "analyzers/TypeofAnalyzer.hxx"
#include "tools.hxx"
#include "string.hxx"

namespace analysis
{
bool TypeofAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs != 1)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    if (args.size() != 1)
    {
        return false;
    }

    ast::Exp * arg = args.back();
    arg->accept(visitor);
    const std::wstring & str = visitor.getResult().getType().getScilabString();

    if (!str.empty())
    {
        TIType type(visitor.getGVN(), TIType::STRING);
        Result & res = e.getDecorator().setResult(type);
        res.getConstant() = new types::String(str.c_str());
        e.getDecorator().setCall(L"typeof");
        visitor.setResult(res);
        return true;
    }
    else
    {
        visitor.getDM().releaseTmp(visitor.getResult().getTempId(), arg);
        return false;
    }
}
}
