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

#include <string>

#include "scilabWrite.hxx"
#include "output/SLintScilabResult.hxx"
#include "checkers/SLintChecker.hxx"

namespace slint
{

SLintScilabResult::~SLintScilabResult() { }

    void SLintScilabResult::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::wstring & msg)
{
    auto & mmap = results[context.getFilename()];
    mmap.emplace(loc, msg);
}

void SLintScilabResult::finalize()
{
    for (const auto & p1 : results)
    {
        std::wstring str = L"In " + p1.first + L":\n";
        scilabWriteW(str.c_str());
        for (const auto & p2 : p1.second)
        {
            std::wstring str = L"  At line " + std::to_wstring(p2.first.first_line) + L": " + p2.second + L"\n";
            scilabWriteW(str.c_str());
        }
    }
}

} // namespace slint
