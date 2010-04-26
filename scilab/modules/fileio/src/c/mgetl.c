/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "mgetl.h"
#include "filesmanagement.h"
#include "mopen.h"
#include "MALLOC.h"
#include "BOOL.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
#define LINE_MAX 4096
#define CR '\r'
#define LF '\n'
#define EMPTYSTR ""
/*--------------------------------------------------------------------------*/
static char *removeEOL(char *_inString);
static char *convertAnsiToUtf(char *_inString);
/*--------------------------------------------------------------------------*/
char **mgetl(int fd, int nbLinesIn, int *nbLinesOut, int *ierr)
{
    char **strLines = NULL;
    FILE *fa = NULL;

    *ierr = MGETL_ERROR;
    *nbLinesOut = 0;

    fa = GetFileOpenedInScilab(fd);

    if (fa)
    {
        char Line[LINE_MAX * 2];
        int nbLines = 0;

        if (nbLinesIn < 0)
        {
            strLines = (char **)MALLOC(sizeof(char *));
            if (strLines == NULL)
            {
                *nbLinesOut = 0;
                *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                return NULL;
            }

            while ( fgets ( Line, sizeof(Line), fa ) != NULL )
            {
                nbLines++;
                strLines = (char **)REALLOC(strLines, nbLines * sizeof(char *));
                if (strLines == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }

                strLines[nbLines - 1] = convertAnsiToUtf(removeEOL(Line)); 
                if (strLines[nbLines - 1] == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }
                strcpy(Line, EMPTYSTR);
            }
            *nbLinesOut = nbLines;
            *ierr = MGETL_NO_ERROR;
        }
        else
        {
            if (nbLinesIn == 0)
            {
                *ierr = MGETL_EOF;
                *nbLinesOut = 0;
                if (strLines) 
                {
                    FREE(strLines);
                }
                strLines = NULL;
            }
            else
            {
                BOOL bContinue = TRUE;
                strLines = (char **)MALLOC(sizeof(char *) * nbLinesIn);
                if (strLines == NULL)
                {
                    *nbLinesOut = 0;
                    *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                    return NULL;
                }

                do 
                {
                    if (nbLines < nbLinesIn)
                    {
                        if ( fgets ( Line, sizeof(Line), fa ) != NULL)
                        {
                            nbLines++;
                            strLines[nbLines - 1] = convertAnsiToUtf(removeEOL(Line)); 
                            if (strLines[nbLines - 1] == NULL)
                            {
                                *nbLinesOut = 0;
                                *ierr = MGETL_MEMORY_ALLOCATION_ERROR;
                                return NULL;
                            }
                            strcpy(Line, EMPTYSTR);
                        }
                        else
                        {
                            bContinue = FALSE;
                        }
                    }
                    else
                    {
                        bContinue = FALSE;
                    }
                } while (bContinue);

                *nbLinesOut = nbLines;
                *ierr = MGETL_NO_ERROR;
            }
        }
    }
    return strLines;
}
/*--------------------------------------------------------------------------*/
char *removeEOL(char *_inString)
{
    if (_inString)
    {
        int len = (int)strlen(_inString);

        if ((_inString[len - 1] == LF) && (len > 0))
        {
            _inString[len - 1] = 0; 
            len = (int)strlen(_inString);
        }
        
        if ((_inString[len - 1] == CR) && (len > 0))
        {
            _inString[len - 1] = 0; 
        }
    }
    return _inString;
}
/*--------------------------------------------------------------------------*/
/*
* convert ansi to Utf
* linux, mac conversion is already made by fgets
* windows need to do conversion
*/
char *convertAnsiToUtf(char *_inString)
{
    char *outString = NULL;
    if (_inString)
    {
#ifdef _MSC_VER
        if (IsValidUTF8(_inString))
        {
            outString = strdup(_inString);
        }
        else
        {
            /* conversion ANSI to UTF */
            int Len = 0;
            int newLen = 0;   
            BSTR bstrCode = NULL;    

            Len = MultiByteToWideChar(CP_ACP, 0, _inString, lstrlen(_inString), NULL, NULL);    
            bstrCode = SysAllocStringLen(NULL, Len);    
            if (bstrCode)
            {
                MultiByteToWideChar(CP_ACP, 0, _inString, lstrlen(_inString), bstrCode, Len);   
                newLen = WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, outString, 0, NULL, NULL);    
                outString = (char*) MALLOC(newLen + 1);    
                if (outString)
                {
                    WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, outString, newLen, NULL, NULL); 
                }
                else
                {
                    outString = strdup(_inString);
                }
                SysFreeString(bstrCode);
                bstrCode = NULL;

            }
            else
            {
                outString = strdup(_inString);
            }
        }
#else
        // on linux, macos 
        // nothing to do
        outString = strdup(_inString);
#endif
    }
    return outString;
}
/*--------------------------------------------------------------------------*/

