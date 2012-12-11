/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <wchar.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "tostring_common.hxx"
#include "formatmode.h"

extern "C"
{
#include "elem_common.h"
#include "os_swprintf.h"
}

#define EXPOSANT_SIZE 2         //exposant symbol + exposant sign

using namespace std;

void addSign(wostringstream * _postr, double _dblVal, bool _bPrintPlusSign, bool _bPaddSign)
{
    if (_bPrintPlusSign == true)
    {
        *_postr << (_dblVal < 0 ? MINUS_STRING : PLUS_STRING);
    }
    else
    {
        if (_bPaddSign)
        {
            *_postr << (_dblVal < 0 ? MINUS_STRING : NO_SIGN);
        }
        else
        {
            *_postr << (_dblVal < 0 ? MINUS_STRING : L"");
        }
    }
}

//void getDoubleFormat(double _dblVal, int *_piWidth, int *_piPrec, /*in/out*/bool* _pExp)
static void getFormatInternal(double _dblVal, int *_piWidth, int *_piPrec, /*in/out*/bool* _pbExp)
{
    double dblDec = 0;
    double dblEnt = 0;
    double dblAbs = fabs(_dblVal);
    int iNbDigit = 0;
    int iNbDec = 0;

    *_pbExp |= getFormatMode() == 0;
    int iTotalLen = 0;
    int iPrecNeeded = getFormatSize();

    if (ISNAN(_dblVal) || !finite(_dblVal))
    {
        *_piWidth = 5;      //" nan" or " inf"
        *_piPrec = 0;
        return;
    }
    //get integer part and fractionnal part
    dblDec = modf(dblAbs, &dblEnt);

    //compute len of entire part
    if (dblEnt == 0)
    {
        if(dblAbs == 0)
        {
            iNbDigit = 1;
        }
        else
        {
            iNbDigit = (int)floor(log10(dblAbs));
            if(iNbDigit < 0)
            {
                iNbDigit = 1;
            }
        }

        if (iNbDigit >= (iPrecNeeded - EXPOSANT_SIZE) || *_pbExp)
        {//why can be happen ?
            //exponant
            *_pbExp = true;
            int iExpLen = (int)log10((double)iNbDigit) + 1;
            iTotalLen = 1 /*integer before dot */  + EXPOSANT_SIZE + (iExpLen < 2 ? 2 : iExpLen);
            *_piWidth = iPrecNeeded;
            *_piPrec = iPrecNeeded - iTotalLen;
            return;
        }
        else
        {
            iTotalLen = iNbDigit;
        }
    }
    else
    {
        double dblTemp = log10(dblEnt);

        if (dblTemp > (iPrecNeeded - EXPOSANT_SIZE) || *_pbExp)
        {
            //exponant mode
            if (dblTemp == 0)
            {
                dblTemp = 1;    //no incidence on value, just to allow log10(dblTemp)
            }
            *_pbExp = true;
            int iExpLen = (int)log10(dblTemp) + 1;
            iTotalLen = 1 + EXPOSANT_SIZE + (iExpLen < 2 ? 2 : iExpLen);
            *_piWidth = iPrecNeeded - EXPOSANT_SIZE;
            *_piPrec = *_piWidth - iTotalLen;
            return;
        }
        else
        {
            iTotalLen = (int)dblTemp + 1;
        }
    }

    //prepare fractionnal part to precision asked
    double dblScale = pow(10., iPrecNeeded - iTotalLen);

    while (iTotalLen <= iPrecNeeded)
    {
        dblDec = dblDec * dblScale;
        dblDec = floor(dblDec + 0.5);
        dblDec = dblDec / dblScale;

        if (dblDec == 0.)
        {
            break;
        }

        dblDec = dblDec * 10;
        int iFloor = (int)floor(dblDec);

        dblDec -= iFloor;
        iTotalLen++;
        iNbDec++;
        dblScale /= 10;
    }

    *_piWidth = iTotalLen;
    *_piPrec = iNbDec;
}

//void getComplexFormat(double _dblR, double _dblI, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, int *_piPrecR, int *_piPrecI, bool* _pExpR, bool* _pExpI)
void getDoubleFormat(double _dblR, double _dblI, DoubleFormat * _pDF)
{
    getFormatInternal(_dblR, &_pDF->iWidthR, &_pDF->iPrecR, &_pDF->bExpR);
    getFormatInternal(_dblI, &_pDF->iWidthI, &_pDF->iPrecI, &_pDF->bExpI);

    _pDF->iTotalWidth = 0;
    if (_dblI == 0)
    {
        if (_dblR == 0)
        {
            _pDF->iTotalWidth = 1;    //"0"
        }
        else
        {
            _pDF->iWidthR += _pDF->iSignLenR;
            _pDF->iWidthR += _pDF->bPrintPointR ? 1 : 0;
            _pDF->iTotalWidth = _pDF->iWidthR;
            _pDF->iWidthI = 0;
        }
    }
    else
    {
        if (_dblR == 0)
        {
            _pDF->iWidthI += _pDF->iSignLenI;
            _pDF->iWidthI += _pDF->bPrintPointI ? 1 : 0;
            _pDF->iTotalWidth = _pDF->iWidthI + (int)_pDF->wstrSymbolI.size();
            _pDF->iWidthR = 0;
        }
        else
        {
            _pDF->iWidthR += _pDF->iSignLenR;
            _pDF->iWidthR += _pDF->bPrintPointR ? 1 : 0;

            _pDF->iWidthI += _pDF->iSignLenI;
            _pDF->iWidthI += _pDF->bPrintPointI ? 1 : 0;

            _pDF->iTotalWidth = _pDF->iWidthR + SIZE_BETWEEN_REAL_COMPLEX + _pDF->iWidthI + (int)_pDF->wstrSymbolI.size();
        }
    }
}

void addDoubleValue(std::wostringstream * _postr, double _dblValR, double _dblValI, DoubleFormat * _pDF)
{
#define FORMAT_MAX      64
    wchar_t pwstFormat[FORMAT_MAX];
    wchar_t pwstOutput[FORMAT_MAX];     // > @ format max
    wchar_t pwstSign[FORMAT_MAX];


    wostringstream ostemp;

    // Real part

    if(_dblValR != 0 || _dblValI == 0)
    {//do not write 0 unless _dblVal == 0 too
        if (_pDF == NULL)
        {
            return;
        }

        if (_pDF->bPrintPlusSignR)
        {
            os_swprintf(pwstSign, FORMAT_MAX, L"%-*ls", _pDF->iSignLenR, _dblValR < 0 ? MINUS_STRING : PLUS_STRING);
        }
        else
        {
            if (_pDF->bPaddSignR)
            {
                os_swprintf(pwstSign, FORMAT_MAX, L"%-*ls", _pDF->iSignLenR, _dblValR < 0 ? MINUS_STRING : NO_SIGN);
            }
            else
            {
                os_swprintf(pwstSign, FORMAT_MAX, L"%-*ls", _pDF->iSignLenR, _dblValR < 0 ? MINUS_STRING : L"");
            }
        }

        if ((_pDF->bPrintOneR == true) || (isEqual(fabs(_dblValR), 1)) == false)
        {
            //do not print if _bPrintOne == false && _dblVal == 1
            if (ISNAN(_dblValR))
            {
                //NaN
                os_swprintf(pwstOutput, FORMAT_MAX, L"%ls%*ls", pwstSign, _pDF->iPrecR, L"Nan");
            }
            else if (!finite(_dblValR))
            {
                //Inf
                os_swprintf(pwstOutput, FORMAT_MAX, L"%ls%*ls", pwstSign, _pDF->iPrecR, L"Inf");
            }
            else if (_pDF->bExpR)
            {
                double dblAbs = fabs(_dblValR);
                double dblDec = 0;
                double dblEnt = 0;
                double dblTemp = 0;

                dblDec = modf(dblAbs, &dblEnt);
                if (dblEnt == 0)
                {
                    dblTemp = floor(log10(dblDec));
                }
                else
                {
                    dblTemp = log10(dblEnt);
                }
                dblDec = dblAbs / pow(10., (double)(int)dblTemp);
                dblDec = modf(dblDec, &dblEnt) * pow(10., _pDF->iPrecR);

                if (_pDF->bPrintPointR && isEqual(fabs(_dblValR), 0) == false)
                {
                    os_swprintf(pwstFormat, FORMAT_MAX, L"%ls%%#.0f%%0%d.0fD%%+0.2d", pwstSign, _pDF->iPrecR);
                }
                else
                {
                    os_swprintf(pwstFormat, FORMAT_MAX, L"%ls%%.0f%%0%d.0fD%%+0.2d", pwstSign, _pDF->iPrecR);
                }

                os_swprintf(pwstOutput, FORMAT_MAX, pwstFormat, dblEnt, dblDec, (int)dblTemp);
            }
            else
            {
                if (_pDF->bPrintPointR && isEqual(fabs(_dblValR), 0) == false)
                {
                    os_swprintf(pwstFormat, FORMAT_MAX, L"%ls%%#-%d.%df", pwstSign, 0/*_pDF->iWidthR + 10*/, _pDF->iPrecR);
                }
                else
                {
                    os_swprintf(pwstFormat, FORMAT_MAX, L"%ls%%-%d.%df", pwstSign, 0/*_pDF->iWidthR - 2*/, _pDF->iPrecR);  //-2 no point needed
                }

                os_swprintf(pwstOutput, FORMAT_MAX, pwstFormat, fabs(_dblValR));
            }

            ostemp << pwstOutput;
            addSpaces(&ostemp, _pDF->iWidthR - (int)ostemp.str().size());
        }
        else if (wcslen(pwstSign) != 0)
        {
            os_swprintf(pwstOutput, FORMAT_MAX, L"%ls", pwstSign);
            ostemp << pwstOutput;
        }
    }

    if(_dblValI)
    {
        DoubleFormat df     = *_pDF;
        df.bExpR            = _pDF->bExpI;
        df.iWidthR          = _pDF->iWidthI;
        df.iPrecR           = _pDF->iPrecI;
        df.bPrintPointR     = _pDF->bPrintPointI;
        df.bPrintPlusSignR  = _pDF->bPrintPlusSignI;
        df.bPrintOneR       = _pDF->bPrintOneI;
        df.bPaddSignR       = _pDF->bPaddSignI;
        df.iSignLenR        = _pDF->iSignLenI;

        if(_dblValR == 0)
        {// 0 + x*%i
            //remove force write sign flag
            df.bPrintPlusSignR = false;
        }
        else
        {
            addSpaces(&ostemp, _pDF->iSpaceBetweenValues);
        }

        int iTotalWidth = df.iTotalWidth;
        df.iTotalWidth = df.iWidthI;
        addDoubleValue(&ostemp, _dblValI, 0, &df);
        df.iTotalWidth = iTotalWidth;
        ostemp << _pDF->wstrSymbolI;
    }
    
    addSpaces(&ostemp, _pDF->iTotalWidth - (int)ostemp.str().size());
    *_postr << ostemp.str();
}

void addSpaces(wostringstream * _postr, int _iSpace)
{
    for (int i = 0 ; i < _iSpace ; i++)
    {
        *_postr << L" ";
    }
}

void configureStream(wostringstream * _postr, int _iWidth, int _iPrec, char _cFill)
{
    _postr->setf(ios::showpoint);
    _postr->width(_iWidth);
    _postr->precision(_iPrec);
    _postr->fill(_cFill);
}
