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

#include <fstream>
#include <iostream>

#include "parser.hxx"
#include "SLint.hxx"
#include "threadmanagement.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
#include "expandPathVariable.h"
#include "localization.h"
#include "isdir.h"
#include "findfiles.h"
#include "freeArrayOfString.h"
#include "fullpath.h"
#include "PATH_MAX.h"
}

#ifdef _MSC_VER
#define DEFAULT_FILESPEC L"*.*"
#else
#define DEFAULT_FILESPEC L"*"
#endif

namespace slint
{

SLint::SLint(SLintResult & result) : visitor(context, result) { }

void SLint::setOptions(SLintOptions & options)
{
    visitor.setOptions(options);
}

void SLint::check()
{
    for (const auto & scifile : scifiles)
    {
        context.setSciFile(scifile);
        scifile->getTree()->accept(visitor);
    }
}

void SLint::setFiles(const std::wstring & files)
{
    std::vector<std::wstring> v = {files};
    setFiles(v);
}

void SLint::setFiles(types::String * files)
{
    const unsigned size = files->getSize();
    std::vector<std::wstring> filesVector;
    filesVector.reserve(size);

    for (unsigned i = 0; i < size; ++i)
    {
        filesVector.emplace_back(files->get(i));
    }
    setFiles(filesVector);
}

void SLint::setFiles(const std::vector<std::wstring> & files)
{
    for (const auto & file : files)
    {
        std::wstring full = getFullPath(file);
        if (isdirW(full.c_str()))
        {
            collectInDirectory(full);
        }
        else if (hasSuffix(full, L".sci"))
        {
            scifiles.emplace_back(parseFile(full));
        }
    }
}

void SLint::collectInDirectory(const std::wstring & path)
{
    std::wstring _path = path + DIR_SEPARATORW;
    int size = -1;

    wchar_t ** files = findfilesW(_path.c_str(), DEFAULT_FILESPEC, &size, FALSE);
    if (size > 0 && files)
    {
        std::vector<std::wstring> filesVector;
        for (int i = 0; i < size; ++i)
        {
            filesVector.emplace_back(_path + files[i]);
        }
        freeArrayOfWideString(files, size);
        setFiles(filesVector);
    }
}

SciFilePtr SLint::parseFile(const std::wstring & filename)
{
    char * _path = wide_string_to_UTF8(filename.c_str());
    std::ifstream src(_path, ios::in | ios::binary | ios::ate);
    FREE(_path);
    if (src.is_open())
    {
        src.seekg(0, src.end);
        int len = static_cast<int>(src.tellg());
        src.seekg (0, src.beg);
        char * buffer = new char[len + 1];
        buffer[len] = '\0';
        src.read(buffer, len);
        src.close();

        wchar_t * _buffer = to_wide_string(buffer);
        delete[] buffer;
        Parser parser;

        ThreadManagement::LockParser();
        parser.parse(_buffer);

        if (parser.getExitStatus() != Parser::Succeded)
        {
            FREE(_buffer);
            delete parser.getTree();
            ThreadManagement::UnlockParser();
            throw FileException(filename, parser.getErrorMessage());
        }
        else
        {
            ThreadManagement::UnlockParser();
            return SciFilePtr(new SciFile(filename, _buffer, parser.getTree()));
        }
    }

    wchar_t * error = to_wide_string(_("Cannot open the file"));
    std::wstring _error(error);
    FREE(error);

    throw FileException(filename, _error);
}

bool SLint::hasSuffix(const std::wstring & filename, const std::wstring & suffix)
{
    return filename.size() >= suffix.size() && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::wstring SLint::getFullPath(const std::wstring & path)
{
    const unsigned SIZE = PATH_MAX * 4;
    wchar_t * fullpath = (wchar_t *)MALLOC(SIZE * sizeof(wchar_t));
    wchar_t * expandedPath = expandPathVariableW(const_cast<wchar_t *>(path.c_str()));
    std::wstring _fullpath;
    if (get_full_pathW(fullpath, expandedPath, SIZE))
    {
        FREE(expandedPath);
        _fullpath = std::wstring(fullpath);
    }
    else
    {
        _fullpath = std::wstring(expandedPath);
        FREE(expandedPath);
    }
    FREE(fullpath);

    return _fullpath;
}
} // namespace slint
