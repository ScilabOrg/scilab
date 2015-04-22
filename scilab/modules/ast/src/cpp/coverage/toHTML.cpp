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

void CoverResult::toHTML(const std::wstring & outputDir)
{
    std::wostringstream out;
    writeHTMLHeader(out);



}


std::wostringstream & CoverResult::writeHTMLHeader(std::wostringstream & out)
{
    out << L"<html lang=\'en\'>" << std::endl
        << L"<head>" << std::endl
        << L"<title>Scilab's coverage - " << info.macroModule << L"/" << info.macroFile << L"</title>" << std::endl
        << L"</head>" << std::endl
        << L"<body>" << std::endl
        << L"<div class=\'source\'>" << std::endl
        << L"<table>" << std::endl;
}

CoverResult::foo()
{
    char * _path = wide_string_to_UTF8(info.macroFile.c_str());
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
        ast::Parser parser;
        parser.parse(_wstr);
        FREE(_wstr);

        ast::Exp * tree = parser.getTree();
        HTMLCodePrinter printer(std::wcout);
        CodePrinterVisitor visitor(printer);

        tree->accept(visitor);

        delete tree;
    }
}
}
