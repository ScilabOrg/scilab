/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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
#include "scilab_sprintf.hxx"

using namespace types;

extern "C"
{
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "charEncoding.h"
#include "os_wcsdup.h"
}

#include <stdio.h>

wchar_t** scilab_sprintf(wchar_t* _pwstName, wchar_t* _pwstInput, typed_list &in, ArgumentPosition* _pArgs, int _iArgsCount, int* _piOutputRows, int* piNewLine)
{
    wchar_t** pwstOutput        = NULL;
    wchar_t* pwstFirstOutput    = NULL;
    *piNewLine = 0;
    size_t pos = 0;

    //\n \n\r \r \t to string
    //find number of lines
    // replace \\n \\t... by \n \t...
    *_piOutputRows = 1;
    std::wstring wstr(_pwstInput);

    pos = wstr.find(L"\\r\\n");
    while (pos != std::string::npos)
    {
        wstr.replace(pos, 4, L"\n");
        (*_piOutputRows)++;
        pos = wstr.find(L"\\r\\n", pos);
    }

    pos = wstr.find(L"\\n");
    while (pos != std::string::npos)
    {
        wstr.replace(pos, 2, L"\n");
        (*_piOutputRows)++;
        pos = wstr.find(L"\\n", pos);
    }

    pos = wstr.find(L"\\r");
    while (pos != std::string::npos)
    {
        wstr.replace(pos, 2, L"\r");
        pos = wstr.find(L"\\r", pos);
    }

    pos = wstr.find(L"\\t");
    while (pos != std::string::npos)
    {
        wstr.replace(pos, 2, L"\t");
        pos = wstr.find(L"\\t", pos);
    }

    if (wstr[wstr.size() - 1] == L'\n')
    {
        *piNewLine = 1;
        (*_piOutputRows)--;
    }

    pwstFirstOutput = os_wcsdup(wstr.c_str());


    //no arg, just return _pwstInput value
    if (_iArgsCount)
    {
        //store all sub parts of the input string
        //for the input string "bla1 %s bla2 %d bla3"
        //store  :
        //pwstToken[0] : "bla1 "
        //pwstToken[0] : "%s bla2 "
        //pwstToken[2] : "%d bla3"

        size_t iStart       = 0;
        size_t iEnd         = 0;
        int iToken          = 0;
        TokenDef* pToken = new TokenDef[_iArgsCount + 1];
        wchar_t* pwstStart  = pwstFirstOutput;

        bool bFinish = false;
        while (!bFinish)
        {
            wchar_t* pwstEnd = wcsstr(pwstStart + (iToken == 0 ? 0 : 1), L"%");
            if (pwstEnd != NULL)
            {
                if (pwstEnd[1] == L'%')
                {
                    //manage "%%"
                    pwstEnd = wcsstr(pwstEnd + 2, L"%");
                    if (pwstEnd == NULL)
                    {
                        //end of string
                        iStart  = pwstStart - pwstFirstOutput;
                        iEnd = wcslen(pwstFirstOutput);
                        bFinish = true;
                    }
                }
                else
                {
                    iStart  = pwstStart - pwstFirstOutput;
                    iEnd    = pwstEnd - pwstFirstOutput;
                }
            }
            else
            {
                //end of string
                iStart  = pwstStart - pwstFirstOutput;
                iEnd = wcslen(pwstFirstOutput);
                bFinish = true;
            }

            pToken[iToken].pwstToken = new wchar_t[iEnd - iStart + 1];

            wcsncpy(pToken[iToken].pwstToken, pwstFirstOutput + iStart, iEnd - iStart);
            pToken[iToken].pwstToken[iEnd - iStart] = L'\0';

            //identify destination type
            //format : %[flags][width][.precision][length]specifier
            //pToken[iToken].pwstToken

            //find %
            wchar_t* pwstPercent = wcsstr(pToken[iToken].pwstToken, L"%");

            if (pwstPercent != NULL)
            {
                //looking for flags
                if (*(pwstPercent + 1) == L'-' ||
                        *(pwstPercent + 1) == L'+' ||
                        *(pwstPercent + 1) == L' ' ||
                        *(pwstPercent + 1) == L'#' ||
                        *(pwstPercent + 1) == L'0')
                {
                    pwstPercent++;
                }

                //looking for width
                if (*(pwstPercent + 1) == L'*')
                {
                    pwstPercent++;
                }
                else
                {
                    //number
                    while (iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
                    }
                }

                //looking for precision
                if (*(pwstPercent + 1) == L'.')
                {
                    pwstPercent++;
                    while (iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
                    }
                }

                //looking for length
                if (*(pwstPercent + 1) == L'h' ||
                        *(pwstPercent + 1) == L'l' ||
                        *(pwstPercent + 1) == L'L')
                {
                    pToken[iToken].bLengthFlag = true;
                    pwstPercent++;
                }
                else
                {
                    pToken[iToken].bLengthFlag = false;
                }

                wchar_t wcType = *(pwstPercent + 1);
                switch (wcType)
                {
                    case L'i' : //integer
                    case L'd' : //integer
                    case L'o' : //octal
                    case L'u' : //unsigned
                    case L'x' : //hex
                    case L'X' : //HEX
                        if (_pArgs[iToken - 1].type != InternalType::RealDouble)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                            return NULL;
                        }
                        pToken[iToken].outputType = InternalType::RealInt32;
                        break;
                    case L'f' : //float
                    case L'e' : //exp
                    case L'E' : //EXP
                    case L'g' : //shorter between float or exp
                    case L'G' : //shorter between float or EXP
                        if (_pArgs[iToken - 1].type != InternalType::RealDouble)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                            return NULL;
                        }
                        pToken[iToken].outputType = InternalType::RealDouble;
                        break;
                    case L's' :
                    case L'c' :
                        if (_pArgs[iToken - 1].type != InternalType::RealString)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                            return NULL;
                        }
                        pToken[iToken].outputType = InternalType::RealString;
                        break;
                    default :
                        //houston ...
                        break;
                }

            }

            pwstStart = pwstEnd;
            iToken++;
        }

        FREE(pwstFirstOutput);
        pwstFirstOutput = NULL;

        int iLoop = 1;
        if (in.size() > 1)
        {
            iLoop = in[1]->getAs<GenericType>()->getRows();
            for (int i = 2 ; i < in.size() ; i++)
            {
                iLoop = Min(iLoop, in[i]->getAs<GenericType>()->getRows());
            }
        }

        if (*piNewLine || (*_piOutputRows) > 1)
        {
            (*_piOutputRows) *= iLoop;
        }

        pwstFirstOutput = (wchar_t*)MALLOC(sizeof(wchar_t*) * iLoop * bsiz);
        memset(pwstFirstOutput, 0x00, sizeof(wchar_t*) * iLoop * bsiz);
        for (int j = 0 ; j < iLoop ; j++)
        {
            //copy the 0th token
            wcscat(pwstFirstOutput, pToken[0].pwstToken);

            //start at 1, the 0th is always without %
            for (int i = 1 ; i < _iArgsCount + 1 ; i++)
            {
                wchar_t pwstTemp[bsiz];
                void* pvVal = NULL;
                if (_pArgs[i - 1].type == InternalType::RealDouble)
                {
                    double dblVal = in[_pArgs[i - 1].iArg]->getAs<Double>()->get(j, _pArgs[i - 1].iPos);
                    if (pToken[i].outputType == InternalType::RealDouble)
                    {
                        swprintf(pwstTemp, bsiz, pToken[i].pwstToken, dblVal);
                    }
                    else if (pToken[i].outputType == InternalType::RealInt32)
                    {
                        swprintf(pwstTemp, bsiz, pToken[i].pwstToken, (int)dblVal);
                    }
                }
                else if (_pArgs[i - 1].type == InternalType::RealString)
                {
                    wchar_t* pwstStr = in[_pArgs[i - 1].iArg]->getAs<types::String>()->get(j, _pArgs[i - 1].iPos);
                    int posC = (int)wcscspn(pToken[i].pwstToken, L"c");
                    int posS = (int)wcscspn(pToken[i].pwstToken, L"s");
                    if (!posS || !posC)
                    {
                        return NULL;
                    }
                    if (posC < posS)
                    {
                        if (pToken[i].bLengthFlag == false)
                        {
                            //replace %c by %lc to wide char compatibility
                            int sizeTotal = (int)wcslen(pToken[i].pwstToken);
                            wchar_t* pwstToken = (wchar_t*)MALLOC(sizeof(wchar_t) * (sizeTotal + 2));
                            wcsncpy(pwstToken, pToken[i].pwstToken, posC);
                            pwstToken[posC] = L'l';
                            wcsncpy(&pwstToken[posC + 1], &pToken[i].pwstToken[posC], sizeTotal - posC);
                            pwstToken[sizeTotal + 1]  = L'\0';
                            swprintf(pwstTemp, bsiz, pwstToken, pwstStr[0]);
                            FREE(pwstToken);
                        }
                        else
                        {
                            swprintf(pwstTemp, bsiz, pToken[i].pwstToken, pwstStr[0]);
                        }
                    }
                    else //'s'
                    {
                        if (pToken[i].bLengthFlag == false)
                        {
                            //replace %s by %ls to wide char compatibility
                            int sizeTotal = (int)wcslen(pToken[i].pwstToken);
                            wchar_t* pwstToken = (wchar_t*)MALLOC(sizeof(wchar_t) * (sizeTotal + 2));
                            wcsncpy(pwstToken, pToken[i].pwstToken, posS);
                            pwstToken[posS] = L'l';
                            wcsncpy(&pwstToken[posS + 1], &pToken[i].pwstToken[posS], sizeTotal - posS);
                            pwstToken[sizeTotal + 1]  = L'\0';
                            swprintf(pwstTemp, bsiz, pwstToken, pwstStr);
                            FREE(pwstToken);
                        }
                        else
                        {
                            swprintf(pwstTemp, bsiz, pToken[i].pwstToken, pwstStr);
                        }
                    }
                }
                else
                {
                    //impossible but maybe in the futur
                }

                wcscat(pwstFirstOutput, pwstTemp);
            }
        }
    }


    // split output string
    std::wstring wstrSplit(pwstFirstOutput);
    pwstOutput = (wchar_t**)MALLOC((*_piOutputRows) * sizeof(wchar_t*));

    int i = 0;
    pos = wstrSplit.find(L"\n");
    while (pos != std::string::npos)
    {
        pwstOutput[i] = (wchar_t*)MALLOC((pos + 1) * sizeof(wchar_t));
        wcsncpy(pwstOutput[i], wstrSplit.c_str(), pos);
        pwstOutput[i][pos] = '\0';
        wstrSplit = wstrSplit.substr(pos + 1);
        pos = wstrSplit.find(L"\n");
        i++;
    }

    if (i == (*_piOutputRows) - 1)
    {
        pwstOutput[i] = os_wcsdup(wstrSplit.c_str());
    }

    return pwstOutput;
}
/*--------------------------------------------------------------------------*/
