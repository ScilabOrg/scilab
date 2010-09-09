/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <stdio.h>
#include <string.h>
#include "sciprint.h"
#include "diary.h"
#include "stack-def.h" /* bsiz */
#include "scilabmode.h"
#include "../../console/includes/ConsolePrintf.h"
#ifdef _MSC_VER
#include "TermPrintf.h"
#endif
#include "MALLOC.h"
#include "charEncoding.h"
#include "getNumberOfArgsAfterFormat.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif
#define MAXPRINTF bsiz /* bsiz size of internal chain buf */
/*--------------------------------------------------------------------------*/ 
/* sciprint uses scivprint */
/* scivprint uses printf_scilab */
/*--------------------------------------------------------------------------*/ 
/**
* print a string 
* @param[in] buffer to disp
*/
static void printf_scilab(char *buffer);
/*--------------------------------------------------------------------------*/ 
void sciprint(const char *fmt,...)
{
    va_list ap;

    va_start(ap, fmt);
    scivprint(fmt, ap);
    va_end (ap);
}
/*--------------------------------------------------------------------------*/ 
int scivprint(const char *fmt,va_list args)
{
    int count = 0;
    int nbArgs = 0;

    if (fmt == NULL) return -1;

    nbArgs = getNumberOfArgsAfterFormat(fmt, args);

    if (nbArgs > 0)
    {
        static char s_buf[MAXPRINTF];

        va_list savedargs;
        va_copy(savedargs, args);

        count= vsnprintf(s_buf,MAXPRINTF-1, fmt, args );
        if (count == -1) s_buf[MAXPRINTF-1]='\0';

        printf_scilab(s_buf);

        va_end(savedargs);

    }
    else if (nbArgs == 0)
    {
        printf_scilab((char*)fmt);
        count = strlen(fmt);
    }

    return count;
}
/*--------------------------------------------------------------------------*/ 
static void printf_scilab(char *buffer)
{
    if (buffer)
    {
        wchar_t *wcBuffer = NULL;
        if (getScilabMode() == SCILAB_STD)
        {
            ConsolePrintf(buffer);
        }
        else
        {
#ifdef _MSC_VER
            TermPrintf_Windows(buffer);
#else
            printf("%s", buffer);
#endif
        }

        wcBuffer = to_wide_string(buffer);
        if (wcBuffer)
        {
            diaryWrite(wcBuffer, FALSE);
            FREE(wcBuffer);
            wcBuffer = NULL;
        }
    }
}
/*--------------------------------------------------------------------------*/ 
