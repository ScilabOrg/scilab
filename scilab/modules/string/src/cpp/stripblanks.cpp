/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "stripblanks.hxx"
/*--------------------------------------------------------------------------*/
#define BLANK_CHARACTER ' '
#define TAB_CHARACTER 9
/*--------------------------------------------------------------------------*/
static char* substr(const char *str, unsigned int startPos, unsigned  int endPos);
/*--------------------------------------------------------------------------*/
String * stripblanks(String *InputStrings, Bool bWithTAB)
{
    String *pOutputStrings = new String(InputStrings->rows_get(), 
                                        InputStrings->cols_get());
    if (pOutputStrings)
    {
        pOutputStrings->string_set(InputStrings->string_get());

        for (int x = 0; x < InputStrings->size_get(); x++)
        {
            int lenghtInput_One = 0;
            int lenghtInput_Two = 0;
            int i = 0;

            char *pStr = InputStrings->string_get(x);
            /* search character ' ' or TAB from end of the string */
            for ( i = (int)strlen(pStr) - 1 ; i > 0 ; i--)
            {
                if (bWithTAB.bool_get())
                {
                    if ( (pStr[i] == (char)BLANK_CHARACTER) ||
                        (pStr[i] == (char)TAB_CHARACTER) )
                    {
                        continue;
                    }
                    else
                    {
                        lenghtInput_Two = i;
                        break;
                    }
                }
                else
                {
                    if (pStr[i] == (char)BLANK_CHARACTER)
                    {
                        continue;
                    }
                    else
                    {
                        lenghtInput_Two = i;
                        break;
                    }
                }
            }
            /* search character ' ' or TAB from beginning of the string */
            if (lenghtInput_Two > 0)
            {
                for (i = 0;i<(int)strlen(pStr); i++)
                {
                    if (bWithTAB.bool_get())
                    {
                        if ( (pStr[i] == (char)BLANK_CHARACTER) ||
                            (pStr[i] == (char)TAB_CHARACTER) )
                        {
                            continue;
                        }
                        else
                        {
                            lenghtInput_One = i;
                            break;
                        }
                    }
                    else
                    {
                        if (pStr[i] == (char)BLANK_CHARACTER)
                        {
                            continue;
                        }
                        else
                        {
                            lenghtInput_One = i;
                            break;
                        }
                    }
                }
            }

            if (lenghtInput_One <= lenghtInput_Two )
            {
                if( (lenghtInput_Two == 0) && ( (pStr[0] == (char)BLANK_CHARACTER ) ||
                    (bWithTAB.bool_get() && (pStr[0] == (char)TAB_CHARACTER)) ) )
                {
                    pOutputStrings->string_set(x, "\0");
                }
                else
                {
                    /*Get the substring without tabs*/ 
                    char * replacedstr = substr(pStr, lenghtInput_One, lenghtInput_Two+1 );
                    /*To add the substring into the output matrix*/
                    pOutputStrings->string_set(x, replacedstr);
                    if (replacedstr) {FREE(replacedstr);replacedstr = NULL;}
                }
            }
        }
    }
    return pOutputStrings;
}
/*--------------------------------------------------------------------------*/
static char* substr(const char *str, unsigned startPos, unsigned endPos)
{
    unsigned n = endPos - startPos;
    char *stbuf = NULL;
    stbuf = (char*)MALLOC(sizeof(char)*(n + 1));

    if (stbuf)
    {
        if ( strcmp(str, "") != 0 )
        {
            strncpy(stbuf, str + startPos, n);   /*Put a part of str into stbuf*/
            stbuf[n] = 0;
        }
        else strcpy(stbuf, "");
    }
    return stbuf;
}
/*------------------------------------------------------------------------*/
