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

#include "output/SLintXmlResult.hxx"
#include "checkers/SLintChecker.hxx"
#include "SLint.hxx"
#include "SciFile.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h"
#include "localization.h"
}

namespace slint
{

SLintXmlResult::SLintXmlResult(const std::wstring & _path) : current(nullptr), path(_path)
{
    const std::wstring fullpath = SLint::getFullPath(path);
    char * __path = wide_string_to_UTF8(fullpath.c_str());
    out = new std::ofstream(__path, std::ios::out);
    FREE(__path);
    if (!out->is_open())
    {
        delete out;
        out = nullptr;
    }
    else
    {
        (*out) << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
               << "<SLintResult>\n";
    }
}

SLintXmlResult::~SLintXmlResult()
{
    if (out)
    {
        out->close();
        delete out;
    }
}

void SLintXmlResult::finalize()
{
    (*out) << "</SLintResult>\n";
    out->close();
    delete out;
    out = nullptr;
}

void SLintXmlResult::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::wstring & msg)
{
    if (context.getSciFile().get() != current.get())
    {
        if (current.get())
        {
            (*out) << "  </File>\n";
        }
        current = context.getSciFile();
        print(current);
    }
    print(loc, checker, msg);
}

void SLintXmlResult::print(const SciFilePtr & file)
{
    char * _file = wide_string_to_UTF8(file->getFilename().c_str());
    (*out) << "  <File name=\"" << _file << "\">\n";
    FREE(_file);
}

void SLintXmlResult::print(const Location & loc, const SLintChecker & checker, const std::wstring & msg)
{
    (*out) << "    <Result>\n";
    print(loc);
    print(checker);
    print(msg);
    (*out) << "    </Result>\n";
}

void SLintXmlResult::print(const Location & loc)
{
    (*out) << "      <Location first_line=\"" << loc.first_line
           << "\" first_column=\"" << loc.first_column
           << "\" last_line=\"" << loc.last_line
           << "\" last_column=\"" << loc.last_column
           << "\"/>\n";
}

void SLintXmlResult::print(const SLintChecker & checker)
{
    (*out) << "      <Checker name=\"" << checker.getName()
           << "\"/>\n";
}

void SLintXmlResult::print(const std::wstring & msg)
{
    std::wstring xmlMsg = replaceByEntities(msg);
    char * _msg = wide_string_to_UTF8(xmlMsg.c_str());
    (*out) << "      <Message text=\"" << _msg
           << "\"/>\n";
    FREE(_msg);
}

std::wstring SLintXmlResult::replaceByEntities(const std::wstring & seq)
{
    std::vector<wchar_t> buf;
    for (auto c : seq)
    {
        if (c == L'<')
        {
            pushEntity(buf, L"&#0060;", 7);
        }
        else if (c == L'>')
        {
            pushEntity(buf, L"&#0062;", 7);
        }
        else if (c == L'\'')
        {
            pushEntity(buf, L"&#0039;", 7);
        }
        else if (c == L'\"')
        {
            pushEntity(buf, L"&#0034;", 7);
        }
        else if (c == L'&')
        {
            pushEntity(buf, L"&#0038;", 7);
        }
        else
        {
            buf.push_back(c);
        }
    }

    return std::wstring(buf.begin(), buf.end());
}

} // namespace slint

/*

<SLintResult date="">
  <File path="">
    <Result>
      <Location first_line="" first_column="" last_line="" last_column=""/>
      <Checker name="" />
      <Message text="" />
    </Result>
    ...
  </File>
  ...
</SLintResult>

 */
