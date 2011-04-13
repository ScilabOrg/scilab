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
#include "scilab_sscanf.hxx"
#include "int.hxx"

extern "C"
{
#include <stdio.h>
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "charEncoding.h"
#include "BOOL.h"
}

wchar_t* findChars(wchar_t*); // Use to find what is inside the [] when the format is %[...].

int scilab_sscanf(wchar_t* _wcsFormat, wchar_t* _wcsData, int _iIterrator, int _iNiter, std::vector<types::InternalType*> *_pITOut)
{
    int i                       = 0;
    int j                       = 0;
    int iSaveIPos               = 0;
    int nbrOfDigit              = 0;
    int dims                    = 2;
    int dimsArray[2]            = {_iNiter,1};
    BOOL bStar                  = FALSE;
    int base                    = 0;
    wchar_t wcsLLH              = L' ';
    wchar_t* wcsData            = _wcsData;

    while(i < (int)wcslen(_wcsFormat))
    {
        if(iswdigit(_wcsFormat[i]))
        {
            nbrOfDigit = wcstol(&_wcsFormat[i], NULL, 10);
            while(iswdigit(_wcsFormat[i]))
            {
                i++;
            }
        }
        else switch(_wcsFormat[i])
        {
            case L' ' :
            case L'\n':
            case L'\t': i++;
            break;
            case L'%' : iSaveIPos = i; i++;
            break;
            case L'*' : bStar = TRUE; i++;
            break;
            case L'h' :
            case L'l' :
            case L'L' : wcsLLH = _wcsFormat[i]; i++;
            break;
            case L'c' :
            {
                if(_iIterrator == 0 && !bStar)
                {
                    types::String* pS = new types::String(dims,dimsArray);
                    for(int k = 0 ; k < pS->getSize(); k++)
                    {
                        pS->set(k, L"");
                    }
                    _pITOut->push_back(pS);
                }

                if(wcsData != NULL && wcsData[0] != L'\0')
                {
                    wchar_t wcSingleData[2];
                    wcSingleData[0] = wcsData[0];
                    wcSingleData[1] = 0;

                    if(!bStar)
                    {
                        (*_pITOut)[j]->getAs<types::String>()->set(_iIterrator, wcSingleData);
                        j++;
                    }

                    wcsData++;
                }

                i++;
                bStar = FALSE;
            }
            break;
            case L's' :
            {
                if(_iIterrator == 0 && !bStar)
                {
                    types::String* pS = new types::String(dims,dimsArray);
                    for(int k = 0 ; k < pS->getSize(); k++)
                    {
                        pS->set(k, L"");
                    }
                    _pITOut->push_back(pS);
                }

                if(wcsData != NULL && wcsData[0] != L'\0')
                {
                    wchar_t* wcsSingleData = NULL;
                    wchar_t* wcsRes = NULL;
                    wchar_t seps[]   = L" ,\t\n";
                    wcsRes = wcstok(wcsData, seps, &wcsData);

                    if(nbrOfDigit)
                    {
                        wcsSingleData = (wchar_t*)MALLOC(sizeof(wchar_t) * (nbrOfDigit + 1));
                        memcpy(wcsSingleData, wcsRes, sizeof(wchar_t) * nbrOfDigit);
                        wcsSingleData[nbrOfDigit] = L'\0';
                        nbrOfDigit = 0;
                    }
                    else
                    {
                        wcsSingleData = wcsRes;
                    }

                    if(!bStar)
                    {
                        (*_pITOut)[j]->getAs<types::String>()->set(_iIterrator, wcsSingleData);
                        j++;
                    }

                    if(nbrOfDigit)
                    {
                        FREE(wcsSingleData);
                    }
                }

                i++;
                bStar = FALSE;
            }
            break;
            case L'[' :
            {
                wchar_t* wcsInside          = NULL;
                wchar_t* wcsCpyFormat       = NULL;
                i++;

                wcsCpyFormat = (wchar_t*)MALLOC((wcslen(_wcsFormat) - i + 1) * sizeof(wchar_t));
                memcpy(wcsCpyFormat, &_wcsFormat[i], (wcslen(_wcsFormat) - i) * sizeof(wchar_t));
                wcsCpyFormat[wcslen(_wcsFormat) - i] = L'\0';

                wcsInside = wcstok(wcsCpyFormat,L"]",&wcsCpyFormat);
                i += wcslen(wcsInside) + 1; // +1 => ]

                if(_iIterrator == 0 && !bStar)
                {
                    types::String* pS = new types::String(dims,dimsArray);
                    for(int k = 0 ; k < pS->getSize(); k++)
                    {
                        pS->set(k, L"");
                    }
                    _pITOut->push_back(pS);
                }

                if(wcsData != NULL && wcsData[0] != L'\0')
                {
                    unsigned int iPos           = 0;
                    wchar_t* wcsSingleData      = NULL;
                    wchar_t* wcsRes             = NULL;
                    wchar_t* wscToFind          = NULL;

                    wscToFind = findChars(wcsInside);
                    if(wscToFind == NULL)
                    {
                        // MALLOC error
                        return -1;
                    }
                    
                    iPos = wcsspn(wcsData, wscToFind);
                    if(iPos == 0)
                    {
                        // The string begins with a character which is not in wscToFind
                        return -1;
                    }
                    else
                    {
                        wcsRes = (wchar_t*)MALLOC((iPos + 1) * sizeof(wchar_t));
                        memcpy(wcsRes, wcsData, iPos * sizeof(wchar_t));
                        wcsRes[iPos] = '\0';

                        FREE(wcsInside);

                        if(nbrOfDigit)
                        {
                            wcsSingleData = (wchar_t*)MALLOC(sizeof(wchar_t) * (nbrOfDigit + 1));
                            memcpy(wcsSingleData, wcsRes, sizeof(wchar_t) * nbrOfDigit);
                            wcsSingleData[nbrOfDigit] = L'\0';
                            wcsData += nbrOfDigit;
                        }
                        else
                        {
                            wcsSingleData = wcsRes;
                            wcsData += iPos;
                        }
                    }

                    if(!bStar)
                    {
                        if(wcsSingleData != NULL)
                        {
                            (*_pITOut)[j]->getAs<types::String>()->set(_iIterrator, wcsSingleData);
                        }
                        j++;
                    }

                    if(nbrOfDigit)
                    {
                        FREE(wcsSingleData);
                    }
                }
                
                nbrOfDigit  = 0;
                bStar       = FALSE;
            }
            break;
            case L'x' :
            case L'X' : base += 6; // 6 + 2 + 8 = 16
            case L'i' :
            case L'd' : base += 2; // 2 + 8 = 10
            case L'o' : base += 8; // 8 = 8 :D
            {
                if(_iIterrator == 0 && !bStar)
                {
                    switch(wcsLLH)
                    {
                        case L'h' :
                        {
                            types::Int16* pInt16 = new types::Int16(dims,dimsArray);
                            for(int k = 0; k < pInt16->getSize(); k++)
                            {
                                pInt16->set(k, 0);
                            }
                            _pITOut->push_back(pInt16);
                        }
                        break;
                        case L'l' :
                        case L'L' :
                        {
                            types::Int64* pInt64 = new types::Int64(dims,dimsArray);
                            for(int k = 0; k < pInt64->getSize(); k++)
                            {
                                pInt64->set(k, 0);
                            }
                            _pITOut->push_back(pInt64);
                        }
                        break;
                        default :
                        {
                            types::Int32* pInt32 = new types::Int32(dims,dimsArray);
                            for(int k = 0; k < pInt32->getSize(); k++)
                            {
                                pInt32->set(k, 0);
                            }
                            _pITOut->push_back(pInt32);
                        }
                    }
                }

                if(wcsData != NULL && wcsData[0] != L'\0')
                {
                    long int iSingleData;
                    while(!iswdigit(wcsData[0]))
                    {
                        switch(wcsData[0])
                        {
                            case L' ' :
                            case L'\n':
                            case L'\t': wcsData++;
                            break;
                            default : return -1;
                        }
                    }

                    if(nbrOfDigit)
                    {
                        int iSizeToConvert  = 0;
                        wchar_t* number     = NULL;

                        while(iswdigit(wcsData[iSizeToConvert]) && iSizeToConvert < nbrOfDigit)
                        {
                            iSizeToConvert++;
                        }

                        number = (wchar_t*)MALLOC((iSizeToConvert + 1) * sizeof(wchar_t));
                        memcpy(number, wcsData, iSizeToConvert * sizeof(wchar_t));
                        number[iSizeToConvert] = L'\0';
                        iSingleData = wcstol(number, NULL, base);
                        wcsData += iSizeToConvert;
                        FREE(number);
                        nbrOfDigit = 0;
                    }
                    else
                    {
                        iSingleData = wcstol(wcsData,&wcsData,base);
                    }

                    if(!bStar)
                    {
                        switch(wcsLLH)
                        {
                            case L'h' : (*_pITOut)[j]->getAs<types::Int16>()->set(_iIterrator, iSingleData);
                            break;
                            case L'l' :
                            case L'L' : (*_pITOut)[j]->getAs<types::Int64>()->set(_iIterrator, iSingleData);
                            break;
                            default : (*_pITOut)[j]->getAs<types::Int32>()->set(_iIterrator, iSingleData);
                        }
                        j++;
                    }
                }
                wcsLLH = L' ';
                i++;
                base = 0;
                bStar = FALSE;
            }
            break;
            case L'e' :
            case L'E' :
            case L'g' :
            case L'G' :
            case L'f' :
            {
                if(_iIterrator == 0 && !bStar)
                {
                    types::Double* pD = new types::Double(dims,dimsArray);
                    for(int k = 0 ; k < pD->getSize(); k++)
                    {
                        pD->set(k, 0);
                    }
                    _pITOut->push_back(pD);
                }

                if(wcsData != NULL && wcsData[0] != L'\0')
                {
                    double dSingleData;
                    while(!iswdigit(wcsData[0]))
                    {
                        switch(wcsData[0])
                        {
                            case L' ' :
                            case L'\n':
                            case L'\t': wcsData++;
                            break;
                            default : return -1;
                        }
                    }

                    if(nbrOfDigit)
                    {
                        int iSizeToConvert  = 0;
                        int iTotalDigits    = 0;
                        wchar_t* number     = NULL;

                        while(  (iswdigit(wcsData[iSizeToConvert]) ||
                                wcsData[iSizeToConvert] == L'.') &&
                                iSizeToConvert < nbrOfDigit)
                        {
                            iSizeToConvert++;
                        }

                        number = (wchar_t*)MALLOC((iSizeToConvert + 1) * sizeof(wchar_t));
                        memcpy(number, wcsData, iSizeToConvert * sizeof(wchar_t));
                        number[iSizeToConvert] = L'\0';
                        dSingleData = wcstod(number, NULL);
                        wcsData += iTotalDigits;
                        FREE(number);
                        nbrOfDigit = 0;
                    }
                    else
                    {
                        dSingleData = wcstod(wcsData,&wcsData);
                    }

                    if(!bStar)
                    {
                        (*_pITOut)[j]->getAs<types::Double>()->set(_iIterrator, dSingleData);
                        j++;
                    }
                }

                i++;
                bStar = FALSE;
            }
            break;
            default :
                printf("format read : %c\n",_wcsFormat[i]);
                return 1;
        }
    }

    return 0;
}

wchar_t* findChars(wchar_t* chars)
{
    unsigned int iIterChars = 0;
    unsigned int iIterRes   = 0;
    int iNbr                = 0;
    int iLen                = 0;
    wchar_t* wcsRes         = NULL;
    int* piPos              = NULL;

    piPos = (int*)MALLOC(wcslen(chars) * sizeof(int));

    for(iIterChars = 0; iIterChars < wcslen(chars); iIterChars++)
    {
        if(chars[iIterChars] == L'-' && iIterChars != 0 && iIterChars != wcslen(chars) - 1)
        {
            int iSize = chars[iIterChars + 1] - chars[iIterChars - 1] - 1;
            if(iSize >= 0)
            {
                iLen += iSize;
                iNbr++;
                piPos[iIterChars] = 1;
            }
            else
            {
                piPos[iIterChars] = 0;
                iLen++;
            }
        }
        else
        {
            piPos[iIterChars] = 0;
            iLen++;
        }
    }

    wcsRes = (wchar_t*)MALLOC((iLen + 1)*sizeof(wchar_t));

    for(iIterChars = 0; iIterChars < wcslen(chars); iIterChars++)
    {
        if(piPos[iIterChars])
        {
            for(int i = 1; i < (chars[iIterChars + 1] - chars[iIterChars - 1]); i++, iIterRes++)
            {
                wcsRes[iIterRes] = chars[iIterChars - 1] + i;
            }
        }
        else
        {
            wcsRes[iIterRes] = chars[iIterChars];
            iIterRes++;
        }
    }
    wcsRes[iLen] = L'\0';

    FREE(piPos);
    return wcsRes;
}
