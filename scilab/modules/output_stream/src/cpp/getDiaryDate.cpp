/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#ifndef _WIN64
#define _USE_32BIT_TIME_T 1
#endif
#endif
/*--------------------------------------------------------------------------*/
#include <sstream>
#include "getDiaryDate.hxx"
/*--------------------------------------------------------------------------*/
extern "C"
{
#include <time.h>
#ifndef _MSC_VER
#include <sys/time.h>
#endif
#include <locale.h>
}
/*--------------------------------------------------------------------------*/
std::wstring getDiaryDate(int format_mode)
{
    std::wstring wstrdate(L"");
    std::wstringstream StrStream;
    time_t tDate;
    time(&tDate);

    switch (format_mode)
    {
        case 0:
        default: // UNIX TIMESTAMP
        {
            StrStream << (unsigned int)tDate;
            wstrdate = StrStream.str();
        }
        break;
        case 1: // http://en.wikipedia.org/wiki/ISO_8601 YYYY-MM-DD hh:mm:ss
        {
            struct tm *nowstruct = localtime(&tDate);

            unsigned int YEAR = 1900 + nowstruct->tm_year;
            unsigned int MONTH = 1 + nowstruct->tm_mon;
            unsigned int DAY_OF_MONTH = nowstruct->tm_mday;
            unsigned int HOUR_OF_DAY = nowstruct->tm_hour;
            unsigned int MINUTES = nowstruct->tm_min;
            unsigned int SECONDS = nowstruct->tm_sec;

            StrStream << YEAR << L"-" << MONTH << L"-" << DAY_OF_MONTH;
            StrStream << L" ";
            StrStream << HOUR_OF_DAY << L":" << MINUTES << L":" << SECONDS;
            wstrdate = StrStream.str();
        }
        break;
    }
    return wstrdate;
}
/*--------------------------------------------------------------------------*/
