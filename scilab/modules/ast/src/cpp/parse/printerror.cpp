/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *  Copyright (C) 20015 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <fstream>
#include <string>
#include "parser_private.hxx"
extern "C"
{
#include "charEncoding.h"
}

void ParserSingleInstance::PrintError(std::wstring msg)
{
    std::wostringstream ostr;
    char *codeLine = (char *) malloc(4096 * sizeof(char));

    // initialize localized strings
    int iLen = 1;
    std::wstring wstrHead(_W("Scilab Parser Error"));
    std::wstring wstrLine(_W("script"));
    iLen = (std::max)(iLen, (int)wstrLine.length());
    std::wstring wstrLocation(_W("location"));
    iLen = (std::max)(iLen, (int)wstrLocation.length());
    std::wstring wstrError(_W("error"));
    iLen = (std::max)(iLen, (int)wstrError.length());

    /** Print an error message to tell it is a parsing error */
    ostr << wstrHead << std::endl;
    ostr << std::left;

    /** Print where in the script the error is located */
    ostr << L"[";
    ostr.width(iLen);
    ostr << wstrLine;
    ostr << L"] ";

    /*
    ** If the error is a the very beginning of a line
    */
    if (yylloc.first_line == yylloc.last_line
            && yylloc.first_column == 1
            && yylloc.last_column == 1)
    {
        --yylloc.first_line;
    }

    wchar_t* str = to_wide_string(ParserSingleInstance::getCodeLine(yylloc.first_line, &codeLine));
    ostr << str;
    // add EOL only if the code line doesn't already contains it.
    if (wcscmp(str + wcslen(str) - 1, L"\n") != 0)
    {
        ostr << std::endl;
    }
    free(codeLine);
    FREE(str);

    /** Underline what causes the trouble */
    ostr << L"[";
    ostr.width(iLen);
    ostr << wstrLocation;
    ostr << L"] ";
    int i = 0;
    for (i = 1 ; i < yylloc.first_column ; ++i)
    {
        ostr << L" ";
    }
    ostr << L"^";
    for (i = i + 1 ; i < yylloc.last_column ; ++i)
    {
        ostr << L"~";
    }
    if ( yylloc.first_column != yylloc.last_column )
    {
        ostr << L"^" ;
    }
    ostr << std::endl;

    /** Display the Location */
    ostr << L"[";
    ostr.width(iLen);
    ostr << wstrLocation;
    ostr << L"] ";
    ostr << ParserSingleInstance::getFileName() << L" : ";
    ostr << _W("Begins at line ") << yylloc.first_line << L", " << _W("column ") << yylloc.first_column << L" : ";
    ostr << _W("Ends at line ") << yylloc.last_line << L", " << _W("column ") << yylloc.last_column << L".";

    /** Display Parser message  */
    ostr << std::endl;
    ostr << L"[";
    ostr.width(iLen);
    ostr << wstrError;
    ostr << L"] " << msg << std::endl;

    ParserSingleInstance::appendErrorMessage(ostr.str());
}
