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

#ifndef __COV_HTML_CODE_PRINTER_HXX__
#define __COV_HTML_CODE_PRINTER_HXX__

#include <iostream>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CodePrinter.hxx"
#include "CoverResult.hxx"

namespace coverage
{

class CovHTMLCodePrinter : public CodePrinter
{

    std::wostringstream & out;
    unsigned int indentLevel;
    unsigned int counter;
    unsigned lineCount;
    bool isNewLine;
    const ast::Exp * current;
    const ast::Exp * last;
    std::unordered_map<std::wstring, CoverResult> & results;
    std::unordered_set<std::wstring> locals;
    std::stack<std::pair<std::wstring, CoverResult *>> fnStack;
    unsigned int fnId;

public:

    CovHTMLCodePrinter(std::wostringstream & _out, std::unordered_map<std::wstring, CoverResult> & _results) : out(_out), indentLevel(0), counter(0), lineCount(0), isNewLine(true), current(nullptr), last(nullptr), results(_results), fnId(0)
    {
    }

    void handleDefault(const std::wstring & seq) override;
    void handleOperator(const std::wstring & seq) override;
    void handleOpenClose(const std::wstring & seq) override;
    void handleFunctionKwds(const std::wstring & seq) override;
    void handleStructureKwds(const std::wstring & seq) override;
    void handleControlKwds(const std::wstring & seq) override;
    void handleConstants(const std::wstring & seq) override;
    void handleCommands(const std::wstring & seq) override;
    void handleMacros(const std::wstring & seq) override;
    void handleFunctionName(const std::wstring & seq) override;
    void handleFunctionNameDec(const std::wstring & seq) override;
    void handleName(const std::wstring & seq) override;
    void handleInOutArgsDec(const std::wstring & seq) override;
    void handleInOutArgs(const std::wstring & seq) override;
    void handleNumber(const std::wstring & seq) override;
    void handleSpecial(const std::wstring & seq) override;
    void handleString(const std::wstring & seq) override;
    void handleNothing(const std::wstring & seq) override;
    void handleField(const std::wstring & seq) override;
    void handleComment(const std::wstring & seq) override;
    void handleNewLine() override;
    void handleExpStart(const ast::Exp * e) override;
    void handleExpEnd(const ast::Exp * e) override;

    inline std::size_t getIndentSize() const
    {
        return indentLevel * 4;
    }

    inline void incIndent()
    {
        ++indentLevel;
    }

    inline void decIndent()
    {
        --indentLevel;
    }

    inline unsigned int getLineCharCount() const
    {
        return counter;
    }

    inline void close()
    {
        if (lineCount)
        {
            out << L"</pre></td><td></td><td></td>\n</tr>\n";
            out.flush();
        }
    }

    inline static std::wstring replaceByEntities(const std::wstring & seq)
    {
        std::vector<wchar_t> buf;
        // if seq contains 100 chars and there are 8 special chars
        // then it will contain after the loop 148 chars.
        // So we should avoid the buf resizement.
        buf.reserve(1.5 * seq.length());
        for (auto c : seq)
        {
            if (c == L'<')
            {
                pushEntity(buf, L"&#0060;");
            }
            else if (c == L'>')
            {
                pushEntity(buf, L"&#0062;");
            }
            else if (c == L'\'')
            {
                pushEntity(buf, L"&#0039;");
            }
            else if (c == L'\"')
            {
                pushEntity(buf, L"&#0034;");
            }
            else if (c == L'&')
            {
                pushEntity(buf, L"&#0038;");
            }
            else
            {
                buf.push_back(c);
            }
        }

        return std::wstring(buf.begin(), buf.end());
    }

    inline static void getDivPercent(std::wostringstream & out, const unsigned int percent)
    {
        if (percent == 0)
        {
            out << L"<div class=\'fullPercent\'><div class=\'filledPercent2\' style=\'width:" << (100 - percent) << L"%;\'></div></div>";
        }
        else if (percent == 100)
        {
            out << L"<div class=\'fullPercent\'><div class=\'filledPercent1\' style=\'width:" << percent << L"%;\'></div></div>";
        }
        else
        {
            out << L"<div class=\'fullPercent\'><div class=\'filledPercent1\' style=\'width:" << percent << L"%;\'></div><div class=\'filledPercent2\' style=\'width:" << (100 - percent) << L"%;\'></div></div>";
        }
    }

private:

    inline const std::wstring & getCurrentFunctionName() const
    {
        return fnStack.top().first;
    }

    inline CoverResult & getCurrentResult() const
    {
        return *fnStack.top().second;
    }

    inline bool isInsideFunction() const
    {
        return !fnStack.empty();
    }

    inline void count(const std::wstring & seq)
    {
        counter += seq.length();
    }

    inline static void pushEntity(std::vector<wchar_t> & buf, const wchar_t * wstr)
    {
        for (const wchar_t * c = wstr; *c != L'\0'; ++c)
        {
            buf.push_back(*c);
        }
    }

    inline void addNewLineHeader()
    {
        if (isNewLine)
        {
            const std::size_t indent = getIndentSize();
            out << L"<tr class=\'";
            if (current && isInsideFunction())
            {
                if (current->isCommentExp())
                {
                    out << L"none";
                }
                else
                {
                    out << (getCurrentResult().isCovered(current) ? L"cover" : L"uncover");
                }
            }
            else
            {
                out << L"none";
            }
            ++lineCount;
            out <<  L"\' id=\'L" << lineCount << L"\'>\n"
                << L"<td class=\'num\'><a href=\'#L" << lineCount << L"\'>" << lineCount << L"</a></td>\n"
                << L"<td class=\'src\'><pre>" << std::wstring(indent, L' ');

            counter = indent;
            isNewLine = false;
        }
        current = nullptr;
    }
};

} // namespace coverage


#endif // __COV_HTML_CODE_PRINTER_HXX__
