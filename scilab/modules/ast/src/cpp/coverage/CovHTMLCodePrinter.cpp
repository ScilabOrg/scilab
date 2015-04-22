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

#include "CovHTMLCodePrinter.hxx"
#include "cover_tools.hxx"

namespace coverage
{

void CovHTMLCodePrinter::handleDefault(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabdefault\'>"
        << replaceByEntities(seq)
        << L"</span>";
}

void CovHTMLCodePrinter::handleOperator(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilaboperator\'>"
        << replaceByEntities(seq)
        << L"</span>";
}

void CovHTMLCodePrinter::handleOpenClose(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabopenclose\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleFunctionKwds(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    if (seq == L"function")
    {
        const CoverResult & result = getCurrentResult();
        const CoverMacroInfo & info = result.getInfo();
        const std::size_t instrs = info.instrsCount;
        const unsigned int percentInstrs = result.getCovInstrsPercent();
        const std::size_t branches = info.branchesCount;
        const unsigned int percentBranches = result.getCovBranchesPercent();
        const std::wstring did = L"d" + std::to_wstring(fnId);
        const std::wstring fid = L"f" + std::to_wstring(fnId++);

        out << L"<a class=\'linkStats\' onmouseover=\"show(\'" << did << L"\',\'" << fid << L"\')\" onmouseout=\"hide(\'" << did << L"\')\">"
            << L"<div id=\'" << did << L"\' class=\'functionStats\'>"
            << L"<table class=\'functionInfo\'>"
            << L"<tr><td colspan=\'5\'>Macro <span class=\'scilabfunctionid\'>" << getCurrentFunctionName() << L"</span>:</td></tr>"
            << L"<tr><td>&nbsp;&mdash;&nbsp;called:</td><td>" << result.getCounter() << L" time" << tools::getPlural(result.getCounter()) << L"</td><td>&nbsp;&mdash;&nbsp;spent time:</td><td>" << result.getStringTime() << L"</td></tr>"
            << L"<tr><td>&nbsp;&mdash;&nbsp;instrs:</td><td>" << instrs << L"</td><td>&nbsp;&mdash;&nbsp;covered:</td><td>" << percentInstrs << L"%</td><td>";

        getDivPercent(out, percentInstrs);

        out << L"</td></tr>"
            << L"<tr><td>&nbsp;&mdash;&nbsp;branches:</td><td>" << branches << L"</td><td>&nbsp;&mdash;&nbsp;covered:</td><td>" << percentBranches << L"%</td><td>";

        getDivPercent(out, percentBranches);

        out << L"</td></tr>"
            << L"</table>"
            << L"</div>"
            << L"<span id=\'" << fid << L"' class=\'scilabfkeyword\'>function</span></a>";
    }
    else
    {
        out << L"<span class=\'scilabfkeyword\'>"
            << seq
            << L"</span>";
    }
}

void CovHTMLCodePrinter::handleStructureKwds(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabskeyword\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleControlKwds(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabckeyword\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleConstants(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabconstants\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleCommands(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabcommand\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleMacros(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabmacro\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleFunctionName(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabfunctionid\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleFunctionNameDec(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabfunctionid\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleName(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    if (locals.find(seq) != locals.end())
    {
        out << L"<span class=\'scilabinputoutputargs\'>"
            << seq
            << L"</span>";
    }
    else
    {
        out << L"<span class=\'scilabid\'>"
            << seq
            << L"</span>";
    }
}

void CovHTMLCodePrinter::handleInOutArgsDec(const std::wstring & seq)
{
    locals.emplace(seq);
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabinputoutputargs\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleInOutArgs(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabinputoutputargs\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleNumber(const std::wstring & seq)
{
    count(seq);
    out << L"<span class=\'scilabnumber\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleSpecial(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabspecial\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleString(const std::wstring & seq)
{
    addNewLineHeader();
    out << L"<span class=\'scilabstring\'>";
    for (const auto c : seq)
    {
        if (c == L'\'')
        {
            out << L"&#0039;&#0039;";
            counter += 2;
        }
        else if (c == L'\"')
        {
            out << L"&#0034;&#0034;";
            counter += 2;
        }
        else
        {
            if (c == L'<')
            {
                out << L"&#0060;";
            }
            else if (c == L'>')
            {
                out << L"&#0062;";
            }
            else if (c == L'&')
            {
                out << L"&#0038;";
            }
            else
            {
                out << c;
            }
            ++counter;
        }
    }
    out << L"</span>";
}

void CovHTMLCodePrinter::handleNothing(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << replaceByEntities(seq);
}

void CovHTMLCodePrinter::handleField(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabfield\'>"
        << seq
        << L"</span>";
}

void CovHTMLCodePrinter::handleComment(const std::wstring & seq)
{
    addNewLineHeader();
    count(seq);
    out << L"<span class=\'scilabcomment\'>"
        << replaceByEntities(seq)
        << L"</span>";
}

void CovHTMLCodePrinter::handleNewLine()
{
    bool printed = false;
    if (lineCount)
    {
        if (last && isInsideFunction())
        {
            if (last->isCommentExp())
            {
                out << L"</pre></td><td></td>\n";
            }
            else
            {
                if (last->isFunctionDec())
                {
                    out << L"</pre></td><td style=\'background-color: rgba(83, 232, 37, 1);\' class=\'time\'>" << getCurrentResult().getStringTime() << L"</td>\n";
                }
                else
                {
                    unsigned int R = 255 - 83;
                    unsigned int G = 255 - 232;
                    unsigned int B = 255 - 37;
                    const double ratio = getCurrentResult().getTimeRatio(last);
                    R = 255 - (unsigned int)std::round((double)R * ratio);
                    G = 255 - (unsigned int)std::round((double)G * ratio);
                    B = 255 - (unsigned int)std::round((double)B * ratio);
                    const std::wstring color = L"rgba(" + std::to_wstring(R) + L"," + std::to_wstring(G) + L"," + std::to_wstring(B) + L",1);";
                    out << L"</pre></td><td style=\'background-color: " << color << L"\' class=\'time\'>" << getCurrentResult().getStringTime(last) << L"</td>\n";
                }
            }

            if (last->isIfExp())
            {
                const std::vector<uint64_t> stats = getCurrentResult().getBranchesStats(last);
                if (!stats.empty())
                {
                    const uint64_t total = stats[0] + stats[1];
                    double thenbr = stats[0];
                    double elsebr = stats[1];
                    if (total)
                    {
                        thenbr = std::round(100 * thenbr / (double)total);
                        elsebr = 100 - thenbr;
                    }
                    const std::wstring thencls = stats[0] == 0 ? L"null_stats" : L"stats";
                    const std::wstring elsecls = stats[1] == 0 ? L"null_stats" : L"stats";

                    out << L"<td class=\'" << thencls << L"\'>" << thenbr << L"%</td><td class=\'" << elsecls << L"\'>" << elsebr << L"%</td>\n</tr>\n";
                    printed = true;
                }
            }
            else if (last->isForExp() || last->isWhileExp())
            {
                uint64_t count;
                if (getCurrentResult().getLoopStats(last, count))
                {
                    std::wstring loopcls = count == 0 ? L"null_stats" : L"stats";
                    out << L"<td class=\'" << loopcls << L"\' colspan=\'2\'>x" << count << L"</td>\n</tr>\n";
                    printed = true;
                }
            }

            if (!printed)
            {
                out << L"<td></td><td></td>\n</tr>\n";
                printed = true;
            }
        }
        if (!printed)
        {
            out << L"</pre></td><td></td><td></td><td></td>\n</tr>\n";
        }
        out.flush();
    }

    last = nullptr;
    isNewLine = true;
}

void CovHTMLCodePrinter::handleExpStart(const ast::Exp * e)
{
    current = e;
    if (!last && !e->isSeqExp())
    {
        last = e;
    }
    if (e->isFunctionDec())
    {
        const std::wstring & name = static_cast<const ast::FunctionDec *>(e)->getSymbol().getName();
        auto i = results.find(name);
        if (i != results.end())
        {
            fnStack.emplace(name, &i->second);
        }
    }
}

void CovHTMLCodePrinter::handleExpEnd(const ast::Exp * e)
{
    if (e->isFunctionDec())
    {
        fnStack.pop();
    }
}
}
