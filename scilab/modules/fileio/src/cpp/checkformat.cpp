/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types.hxx"
#include "double.hxx"
#include "string.hxx"
#include "checkformat.hxx"

extern "C"
{
#include "stdio.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
}

/***********************************************/
/* format:  %[*][width][modifiers]type ignored */
/* iEtat : |0|1|   2  |     3    |  4 |   5   |*/
/*                                             */
/* Each iEtat manages his etat.                */
/*                                             */
/*           ||         _uiFormat :            */
/*_wcsFormat ||(l| |h|u|x|[;s|o|n;f|g|e|d;i|c) */
/* ==========++==============================  */
/* %d = %d%d || 0 1 0 0 0  0  0  0  0 0  1  0  */
/*        %s || 0 1 0 0 0  1  0  0  0 0  0  0  */
/*        %lf|| 1 0 0 0 0  0  0  1  0 0  0  0  */
/*      %s%lf|| 1 1 0 0 0  1  0  1  0 0  0  0  */
/***********************************************/
    
int checkFormat(wchar_t* _wcsFormat, unsigned int* _uiFormat)
{
    int iNumberPercent  = 0;
    int iEtat           = 5;
    int i               = 0;
    *_uiFormat          = 0; // Use to know if the same format is use in the _wcsFormat. (ex: _wcsFormat = "%d%d%d")
                             // Each bit of _uiFormat is associate to one formate (ex: 000...01 => %c ||000...10 => %d)
                             
    while(i < (int)wcslen(_wcsFormat))
    {
        switch(iEtat)
        {
            case 0 : // %
            {
                switch(_wcsFormat[i])
                {
                    case L'%' :
                        i++;
                        iEtat = 1;
                        iNumberPercent++;
                    break;
                    case L' ' :
                    case L'\n':
                    case L'\t':
                        i++;
                    break;
                    default :
                        return CHECK_ERROR_BAD_FORMAT;
                }
            break;
            }
            case 1 : // *
            {
                if(iswdigit(_wcsFormat[i]))
                {
                    iEtat = 2;
                }
                else switch(_wcsFormat[i])
                {
                    case L'*' :
                        i++;
                        iEtat = 2;
                        iNumberPercent--;
                    break;
                    case L'h' :
                    case L'l' :
                    case L'L' :
                    case L'c' :
                    case L'i' :
                    case L'u' :
                    case L'd' :
                    case L'e' :
                    case L'E' :
                    case L'g' :
                    case L'G' :
                    case L'f' :
                    case L'o' :
                    case L's' :
                    case L'x' :
                    case L'X' :
                    case L'[' :
                    case L'n' :
                        iEtat = 3;
                    break;
                    default :
                        return CHECK_ERROR_BAD_FORMAT;
                }
            break;
            }
            case 2 : // width
            {
                if(iswdigit(_wcsFormat[i]))
                {
                    i++;
                }
                else switch(_wcsFormat[i])
                {
                    case L'h' :
                    case L'l' :
                    case L'L' :
                    case L'c' :
                    case L'i' :
                    case L'u' :
                    case L'd' :
                    case L'e' :
                    case L'E' :
                    case L'g' :
                    case L'G' :
                    case L'f' :
                    case L'o' :
                    case L's' :
                    case L'x' :
                    case L'X' :
                    case L'[' :
                    case L'n' :
                    break;
                    default :
                        return CHECK_ERROR_BAD_FORMAT;
                }
            iEtat = 3;
            break;
            }
            case 3 : // modifiers
            {
                switch(_wcsFormat[i])
                {
                    case L'h' :
                        *_uiFormat |= (1 << 9); // short
                        i++;
                    break;
                    case L'l' :
                    case L'L' :
                        *_uiFormat |= (1 << 11); // long
                        i++;
                    break;
                    case L'i' :
                    case L'u' :
                    case L'd' :
                    case L'x' :
                    case L'X' :
                    case L'o' :
                    case L'e' :
                    case L'E' :
                    case L'g' :
                    case L'G' :
                    case L'f' :
                    case L'c' :
                    case L's' :
                    case L'[' :
                    case L'n' : *_uiFormat |= (1 << 10); // not long && not short
                    break;
                    default :
                        return CHECK_ERROR_BAD_FORMAT;
                }
                iEtat = 4;
            break;
            }
            case 4 : // type
            {
                switch(_wcsFormat[i])
                {
                    case L'c' :
                        if( _wcsFormat[i-1] == L'h' ||
                            _wcsFormat[i-1] == L'l' ||
                            _wcsFormat[i-1] == L'L')
                        {
                            return CHECK_ERROR_BAD_FORMAT;
                        }
                        *_uiFormat |= 1;
                    break;
                    case L'u' :
                        *_uiFormat |= (1 << 8);
                    break;
                    case L'i' :
                    case L'd' :
                        *_uiFormat |= (1 << 1);
                    break;
                    case L'e' :
                    case L'E' :
                        if( _wcsFormat[i-1] == L'h')
                        {
                            return CHECK_ERROR_BAD_FORMAT;
                        }
                        *_uiFormat |= (1 << 2);
                    break;
                    case L'g' :
                    case L'G' :
                        if( _wcsFormat[i-1] == L'h')
                        {
                            return CHECK_ERROR_BAD_FORMAT;
                        }
                        *_uiFormat |= (1 << 3);
                    break;
                    case L'n' : iNumberPercent--;
                    case L'f' :
                        if( _wcsFormat[i-1] == L'h')
                        {
                            return CHECK_ERROR_BAD_FORMAT;
                        }
                        *_uiFormat |= (1 << 4);
                    break;
                    case L'o' :
                        *_uiFormat |= (1 << 5);
                    break;
                    case L's' :
                        if( _wcsFormat[i-1] == L'h')
                        {
                            return CHECK_ERROR_BAD_FORMAT;
                        }
                        *_uiFormat |= (1 << 6);
                    break;
                    case L'[' :
                    {
                        if( _wcsFormat[i-1] == L'h' ||
                            _wcsFormat[i-1] == L'l' ||
                            _wcsFormat[i-1] == L'L')
                        {
                            return CHECK_ERROR_BAD_FORMAT;
                        }

                        wchar_t* wcsCpy = (wchar_t*)MALLOC((wcslen(_wcsFormat)-i+1) * sizeof(wchar_t));
                        memcpy(wcsCpy, &_wcsFormat[i], (wcslen(_wcsFormat)-i) * sizeof(wchar_t));
                        wcsCpy[wcslen(_wcsFormat)-i] = L'\0';
                        wchar_t* wcsInside = wcstok(wcsCpy,L"]",&wcsCpy);
                        i += wcslen(wcsInside);
                        *_uiFormat |= (1 << 6);
                    }
                    break;
                    case L'x' :
                    case L'X' :
                        *_uiFormat |= (1 << 7);
                    break;
                    default :
                        return CHECK_ERROR_BAD_FORMAT;
                }
                i++;
                iEtat = 5;
            }
            break;
            case 5 : // chars ignored
            {
                switch(_wcsFormat[i])
                {
                    case L'%' :
                        iEtat = 0;
                    break;
                    default : i++;
                }
            }    
            break;
        }
    }

    if(iEtat != 5)
    {
        return CHECK_ERROR_INCOMPLETE_FORMAT;
    }

    return iNumberPercent;
}

