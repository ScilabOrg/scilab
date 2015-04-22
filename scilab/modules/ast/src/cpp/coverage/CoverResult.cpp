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

#include "CoverResult.hxx"
#include "cover_tools.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alltypes.hxx"
#include "printvisitor.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
#include "expandPathVariable.h"
}

namespace coverage
{

void CoverResult::populate(const std::vector<Counter>::const_iterator pos, const std::vector<Counter>::const_iterator end)
{
    Location current, prev;
    for (std::vector<Counter>::const_iterator i = pos; i != end; ++i)
    {
        ast::Exp * e = i->getExp();
        times.emplace(e, i->getNanoTime());

        if (!CoverResult::isInside(prev, e->getLocation()))
        {
            addNanoTime(i->getNanoTime());
            prev = e->getLocation();
        }

        if ((e->isSeqExp() || e->isCommentExp()) && e->getParent() && !e->getParent()->isFunctionDec())
        {
            // we have a branch with a parent
            ast::Exp * parent = e->getParent();
            auto j = branches.find(parent);
            if (j == branches.end())
            {
                branches.emplace(parent, std::vector<uint64_t>(1, i->get()));
            }
            else
            {
                j->second.emplace_back(i->get());
            }
            if (parent->isForExp() || parent->isWhileExp())
            {
                loops.emplace(parent, i->get());
            }
        }

        if (i->get())
        {
            // Covered expression
        }
        else
        {
            // Uncovered expression
            if ((e->isSeqExp() || e->isCommentExp()) && e->getParent() && !e->getParent()->isFunctionDec())
            {
                ++uncoveredBranches;
            }
            else
            {
                ++uncoveredInstrs;
                if (!CoverResult::isInside(current, e->getLocation()))
                {
                    unused.insert(e);
                    current = e->getLocation();
                }
            }
        }
    }
}

bool CoverResult::isCovered(const ast::Exp * e) const
{
    if (unused.empty())
    {
        return true;
    }
    auto i = unused.lower_bound(const_cast<ast::Exp *>(e));
    if (i == unused.end())
    {
        return !CoverResult::isInside((*std::prev(i))->getLocation(), e->getLocation());
    }
    else
    {
        if (i == unused.begin())
        {
            return !CoverResult::isInside((*i)->getLocation(), e->getLocation());
        }
        return !CoverResult::isInside((*i)->getLocation(), e->getLocation()) && !CoverResult::isInside((*std::prev(i))->getLocation(), e->getLocation());
    }
}

std::wostream & operator<<(std::wostream & out, const CoverResult & cr)
{
    const std::size_t instrs = cr.info.instrsCount;
    const double percentInstrs = instrs ? 100 * (1 - (double)cr.uncoveredInstrs / (double)instrs) : 100;
    const std::size_t branches = cr.info.branchesCount;
    const double percentBranches = branches ? 100 * (1 - (double)cr.uncoveredBranches / (double)branches) : 100.;
    out << tools::underline(L"Macro " + cr.name)
        << L" -called: " << cr.counter << L" time" << tools::getPlural(cr.counter) << L"." << std::endl
        << L" -instrs: " << instrs << std::endl
        << L" -branches: " << cr.info.branchesCount << std::endl
        << L" -paths: " << cr.info.pathsCount << std::endl
        << L" -covered instrs: " << (instrs - cr.uncoveredInstrs) << L" (" << percentInstrs << L"%)" << std::endl
        << L" -covered branches: " << (branches - cr.uncoveredBranches) << L" (" << percentBranches << L"%)" << std::endl
        << std::endl;

    if (!cr.branches.empty())
    {
        out << tools::underline(L"Branches stats");
        for (const auto & p : cr.branches)
        {
            const std::vector<uint64_t> & counters = p.second;
            const std::size_t size = counters.size();
            out << L"Expr at " << p.first->getLocation() << L" has " << size << (size > 1 ? L" branches" : L" branch") << std::endl;

            if (size > 1)
            {
                uint64_t sum = 0;
                for (const auto & c : counters)
                {
                    sum += c;
                }

                if (sum)
                {
                    for (std::size_t i = 0; i < size; ++i)
                    {
                        const double percent = (100. * (double)counters[i] / (double)sum);
                        out << L"Br " << (i + 1) << L": " << percent << L"% ";
                    }
                    out << std::endl;
                }
            }
        }

        out << std::endl;
    }

    if (cr.counter && !cr.unused.empty())
    {
        out << tools::underline(L"Non covered") << std::endl;

        ast::PrintVisitor print(out);
        for (const auto e : cr.unused)
        {
            out << L"At " << e->getLocation() << L":" << std::endl;
            e->accept(print);
            out << std::endl;
        }
    }

    return out;
}

void CoverResult::toXML(const std::wstring & outputDir)
{
    std::wostringstream out;
    const std::wstring tab1(L"  ");
    const std::wstring tab2(L"    ");
    const std::wstring tab3(L"      ");
    out << L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << std::endl

        << L"<cover name=\"" << name << L"\""
        << L" file=\"" << info.macroFilePath << L"\""
        << L" module=\"" << info.macroModule << L"\""
        << L" instrs_count=\"" << info.instrsCount << L"\""
        << L" branches_count=\"" << info.branchesCount << L"\""
        << L" paths_count=\"" << info.pathsCount << L"\""
        << L">" << std::endl

        << tab1 << L"<result instrs_count=\"" << (info.instrsCount - uncoveredInstrs) << L"\""
        << L" branches_count=\"" << (info.branchesCount - uncoveredBranches) << L"\""
        << L" paths_count=\"" << 0 << L"\""
        << L"/>" << std::endl;

    if (branches.empty())
    {
        out << tab1 << L"<branches/>" << std::endl;
    }
    else
    {
        out << tab1 << L"<branches>" << std::endl;
        for (const auto & p : branches)
        {
            const std::vector<uint64_t> & counters = p.second;
            const std::size_t size = counters.size();
            const Location & loc = p.first->getLocation();
            out << tab2 << L"<exp branches_number=\"" << size << L"\">" << std::endl

                << tab3 << L"<location first_line=\"" << loc.first_line << L"\""
                << L" first_column=\"" << loc.first_column << L"\""
                << L" last_line=\"" << loc.last_line << L"\""
                << L" last_column=\"" << loc.last_column << L"\""
                << L"/>" << std::endl;

            for (std::size_t i = 0; i < size; ++i)
            {
                out << tab3 << L"<branche index=\"" << (i + 1) << L"\""
                    << L" counter=\"" << counters[i] << L"\""
                    << L"/>" << std::endl;
            }

            out << tab2 << L"</exp>" << std::endl;
        }
        out << tab1 << L"</branches>" << std::endl;
    }

    if (unused.empty())
    {
        out << tab1 << L"<uncovered/>" << std::endl;
    }
    else
    {
        out << tab1 << L"<uncovered>" << std::endl;
        for (const auto e : unused)
        {
            const Location & loc = e->getLocation();
            out << tab2 << L"<location first_line=\"" << loc.first_line << L"\""
                << L" first_column=\"" << loc.first_column << L"\""
                << L" last_line=\"" << loc.last_line << L"\""
                << L" last_column=\"" << loc.last_column << L"\""
                << L"/>" << std::endl;
        }
        out << tab1 << L"</uncovered>" << std::endl;
    }

    out << L"</cover>";

    char * code = wide_string_to_UTF8(out.str().c_str());
    wchar_t * _output = expandPathVariableW((wchar_t *)outputDir.c_str());
    const std::wstring filename = std::wstring(_output) + DIR_SEPARATORW + name + L".xml";
    char * _filename = wide_string_to_UTF8(filename.c_str());
    std::fstream file(_filename, std::ios::out | std::ios::trunc);
    file.write(code, std::strlen(code));
    file.close();
    FREE(code);
    FREE(_filename);
    FREE(_output);
}

void CoverResult::toJSON(const std::wstring & outputDir)
{
    std::wostringstream out;
    const std::wstring tab1(L"    ");
    const std::wstring tab2(L"        ");
    const std::wstring tab3(L"            ");
    out << L"{" << std::endl

        << tab1 << L"\"name\": \"" << name << L"\"," << std::endl
        << tab1 << L"\"file\": \"" << info.macroFilePath << L"\"," << std::endl
        << tab1 << L"\"module\": \"" << info.macroModule << L"\"," << std::endl
        << tab1 << L"\"instrs_count\": \"" << info.instrsCount << L"\"," << std::endl
        << tab1 << L"\"branches_count\": \"" << info.branchesCount << L"\"," << std::endl
        << tab1 << L"\"paths_count\": \"" << info.pathsCount << L"\"," << std::endl

        << tab1 << L"\"result\": {" << std::endl
        << tab2 << L"\"instrs_count\": \"" << (info.instrsCount - uncoveredInstrs) << L"\"," << std::endl
        << tab2 << L"\"branches_count\": \"" << (info.branchesCount - uncoveredBranches) << L"\"," << std::endl
        << tab2 << L"\"paths_count\": \"" << 0 << L"\"," << std::endl
        << tab1 << L"}," << std::endl;

    if (branches.empty())
    {
        out << tab1 << L"\"branches\": []," << std::endl;
    }
    else
    {
        out << tab1 << L"\"branches\": [" << std::endl;
        const auto last = std::prev(branches.end());
        for (auto i = branches.begin(), end = branches.end(); i != end; ++i)
        {
            const std::vector<uint64_t> & counters = i->second;
            const std::size_t size = counters.size();
            const Location & loc = i->first->getLocation();
            out << tab2 << L"{" << std::endl
                << tab3 << L"\"location\": { \"first_line\": \"" << loc.first_line << L"\""
                << L", \"first_column\": \"" << loc.first_column << L"\""
                << L", \"last_line\": \"" << loc.last_line << L"\""
                << L", \"last_column\": \"" << loc.last_column << L"\""
                << L"}," << std::endl
                << tab3 << L"\"counters\": [";

            for (std::size_t j = 0; j < size - 1; ++j)
            {
                out << L"\"" << counters[j] << L"\", ";
            }
            out << L"\"" << counters[size - 1] << L"\"]" << std::endl;

            if (i != last)
            {
                out << tab2 << L"}, " << std::endl;
            }
            else
            {
                out << tab2 << L"}" << std::endl;
            }
        }
        out << tab1 << L"]," << std::endl;
    }

    if (unused.empty())
    {
        out << tab1 << L"\"uncovered\": []" << std::endl;
    }
    else
    {
        out << tab1 << L"\"uncovered\": [" << std::endl;
        const auto last = std::prev(unused.end());
        for (auto i = unused.begin(), end = unused.end(); i != end; ++i)
        {
            const Location & loc = (*i)->getLocation();
            out << tab2 << L"{ \"first_line\": \"" << loc.first_line << L"\""
                << L", \"first_column\": \"" << loc.first_column << L"\""
                << L", \"last_line\": \"" << loc.last_line << L"\""
                << L", \"last_column\": \"" << loc.last_column << L"\"";
            if (i != last)
            {
                out << L"}," << std::endl;
            }
            else
            {
                out << L"}" << std::endl;
            }
        }
        out << tab1 << L"]" << std::endl;
    }

    out << L"}";

    char * code = wide_string_to_UTF8(out.str().c_str());
    wchar_t * _output = expandPathVariableW((wchar_t *)outputDir.c_str());
    const std::wstring filename = std::wstring(_output) + DIR_SEPARATORW + name + L".json";
    char * _filename = wide_string_to_UTF8(filename.c_str());
    std::fstream file(_filename, std::ios::out | std::ios::trunc);
    file.write(code, std::strlen(code));
    file.close();
    FREE(code);
    FREE(_filename);
    FREE(_output);
}

void CoverResult::toHTML(const std::wstring & outputDir)
{
    /*
    char * _path = wide_string_to_UTF8(info.macroFilePath.c_str());
    std::ifstream src(_path, ios::in | ios::binary | ios::ate);
    FREE(_path);
        if (src.is_open())
        {
        src.seekg(0, src.end);
        int len = src.tellg();
        src.seekg (0, src.beg);
        char * buffer = new char[len + 1];
        buffer[len] = '\0';
        src.read(buffer, len);
        src.close();

        wchar_t * _wstr = to_wide_string(buffer);
        delete[] buffer;
        Parser parser;
        parser.parse(_wstr);
        FREE(_wstr);

        ast::Exp * tree = parser.getTree();
        std::wostringstream out;

        out << L"<html lang=\'en\'>\n"
    	<< L"<meta charset=\'UTF-8\'>\n"
    	<< L"<head>\n"
    	<< L"<title>Scilab\'s coverage - " << info.macroModule << L"/" << info.macroFilePath << L"</title>\n"
    	<< L"<style type=\'text/css\' media=\'all\'>\n"
    	<< L"@import url(\'./scilab_code.css\');\n"
    	<< L"@import url(\'./style.css\');\n"
    	<< L"</style>\n"
    	<< L"</head>\n"
    	<< L"<body>\n"
    	<< L"<div class=\'source\'>\n"
    	<< L"<table>\n";

        HTMLCodePrinter printer(out, this);
        CodePrinterVisitor visitor(printer);
        tree->accept(visitor);
        printer.close();
        delete tree;

        out << L"</table>\n"
    	<< L"</div>\n"
    	<< L"</body>\n"
    	<< L"</html>\n";

        out.flush();

        char * code = wide_string_to_UTF8(out.str().c_str());
        wchar_t * _output = expandPathVariableW((wchar_t *)outputDir.c_str());
        const std::wstring filename = std::wstring(_output) + DIR_SEPARATORW + name + L".html";
        char * _filename = wide_string_to_UTF8(filename.c_str());
        std::fstream file(_filename, std::ios::out | std::ios::trunc);
        file.write(code, std::strlen(code));
        file.close();
        FREE(code);
        FREE(_filename);
        FREE(_output);
        }
    */
}
}
