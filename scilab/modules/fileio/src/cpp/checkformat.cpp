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

int checkFormat(wchar_t* _wcsFormat, unsigned int* _uiFormat)
{
    int iNumberPercent  = 0;
    int iEtat           = 0;
    int i               = 0;
    *_uiFormat          = 0;

    while(i < (int)wcslen(_wcsFormat))
    {
        switch(iEtat)
        {
            case 0 :
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
            case 1 :
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
                        iEtat = 3;
                    break;
                    case L'c' :
                    case L'i' :
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
                        iEtat = 4;
                    break;
                    default :
                        return CHECK_ERROR_BAD_FORMAT;
                }
            break;
            }
            case 2 :
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
                        iEtat = 3;
                    break;
                    case L'c' :
                    case L'i' :
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
                        iEtat = 4;
                    break;
                    default :
                        return CHECK_ERROR_BAD_FORMAT;
                }
            break;
            }
            case 3 :
            {
                switch(_wcsFormat[i])
                {
                    case L'h' :
                        *_uiFormat |= (1 << 8);
                        i++;
                    break;
                    case L'l' :
                    case L'L' :
                        *_uiFormat |= (1 << 9);
                        i++;
                    break;
                    case L'c' :
                    case L'i' :
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
                    break;
                    default :
                        return CHECK_ERROR_BAD_FORMAT;
                }
                iEtat = 4;
            break;
            }
            case 4 :
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
                iEtat = 0;
            break;
            }
        }
    }

    if(iEtat != 0)
    {
        return CHECK_ERROR_INCOMPLETE_FORMAT;
    }

    return iNumberPercent;
}

