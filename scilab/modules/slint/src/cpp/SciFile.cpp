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

#include "SciFile.hxx"

extern "C"
{
#include "sci_malloc.h"
}

namespace slint
{

PCREMatcher SciFile::commentedLine(L"^[\\t ]*//");
PCREMatcher SciFile::emptyLine(L"^[\\t ]*$");

SciFile::SciFile() : code(nullptr), tree(nullptr) { }

SciFile::SciFile(const std::wstring & _filename, const wchar_t * _code, const ast::Exp * _tree) : filename(_filename), code(_code), tree(_tree)
{
    initLines();
}

SciFile::~SciFile()
{
    FREE(const_cast<wchar_t *>(code));
    delete tree;
}

const std::wstring & SciFile::getFilename() const
{
    return filename;
}

const wchar_t * SciFile::getCode() const
{
    return code;
}

unsigned int SciFile::getCodeLength() const
{
    return codeLength;
}

const ast::Exp * SciFile::getTree() const
{
    return tree;
}

bool SciFile::getPosition(const Location & loc, std::pair<unsigned int, unsigned int> & out) const
{
    if (loc.first_line > lines.size() || loc.last_line > lines.size())
    {
        return false;
    }
    out.first = lines[loc.first_line - 1].first + loc.first_column - 1;
    out.second = lines[loc.last_line - 1].first + loc.last_column - 1;

    return out.first <= codeLength && out.second <= codeLength;
}

bool SciFile::getFromPositionToEOL(const Location & loc, std::pair<unsigned int, unsigned int> & out) const
{
    if (loc.first_line + 1 > lines.size())
    {
        return false;
    }
    out.first = lines[loc.first_line - 1].first + loc.first_column - 1;
    out.second = lines[loc.last_line - 1].second + 1;

    return out.first <= codeLength;
}

bool SciFile::checkLineLength(const unsigned int max, std::vector<unsigned int> & out) const
{
    unsigned int i = 1;
    for (const auto & line : lines)
    {
        if (line.second - line.first + 1 > max)
        {
            out.push_back(i);
        }
        ++i;
    }
    return out.empty();
}

unsigned int SciFile::countLines() const
{
    unsigned int i = 0;
    for (const auto & line : lines)
    {
        if (line.first < line.second && !commentedLine.match(code + line.first, line.second - line.first, false) && !emptyLine.match(code + line.first, line.second - line.first, true))
        {
            ++i;
        }
    }
    return i;
}

void SciFile::initLines()
{
    lines.clear();
    codeLength = 0;
    if (code)
    {
        const wchar_t * p = code;
        lines.emplace_back(0, 0);
        while (*p)
        {
            if (*p == L'\n')
            {
                lines.back().second = p - code - 1;
                lines.emplace_back(p - code + 1, 0);
                ++p;
            }
            else if (*p == L'\r' && *(p + 1) == L'\n')
            {
                lines.back().second = p - code - 1;
                lines.emplace_back(p - code + 2, 0);
                p += 2;
            }
            else
            {
                ++p;
            }
        }

        lines.back().second = p - code;
        codeLength = p - code + 1;
    }

    /*for (const auto & line : lines)
      {
      std::wcerr << line.first << "->" << line.second << std::endl;
      }*/
}


} // namespace slint
