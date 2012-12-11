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

#ifndef __TOSTRING_COMMON_HXX__
#define __TOSTRING_COMMON_HXX__

#include <sstream>
#include <string>
#include "dynlib_types.h"
#include "os_swprintf.h"

#define SIZE_BETWEEN_TWO_VALUES         2
#define SPACE_BETWEEN_TWO_VALUES        L"  "
#define SIZE_BETWEEN_REAL_COMPLEX       1
#define SPACE_BETWEEN_REAL_COMPLEX      L" "
#define SIZE_BOOL                       1
#define SIZE_BETWEEN_BOOL               1
#define SPACE_BETWEEN_BOOL              L" "
#define SIGN_LENGTH                     1
#define NO_SIGN                         L" "
#define MINUS_STRING                    L"-"
#define SIZE_MINUS_STRING               1
#define PLUS_STRING                     L"+"
#define MINUS_STRING_INT                L" -"
#define SYMBOL_I                        L"i"
#define SYMBOL_II                       L"%i"


#define MAX_LINES                       100
#ifndef _MSC_VER
#include <inttypes.h>
#define _abs64(x) llabs(x)
#endif

typedef struct __DOUBLE_FORMAT__
{
    __DOUBLE_FORMAT__() : wstrSymbolI(SYMBOL_I), iTotalWidth(0), iSpaceBetweenValues(1),
            bExpR(false), bExpI(false), 
            iWidthR(0), iWidthI(0), 
            iPrecR(0), iPrecI(0), 
            bPrintPointR(true), bPrintPointI(true), 
            bPrintPlusSignR(false), bPrintPlusSignI(true), 
            bPrintOneR(true), bPrintOneI(false), 
            bPaddSignR(true), bPaddSignI(true), 
            iSignLenR(2), iSignLenI(2) {}
    bool bExpR;
    bool bExpI;
    int iWidthR;
    int iWidthI;
    int iPrecR;
    int iPrecI;
    bool bPrintPointR;
    bool bPrintPointI;
    bool bPrintPlusSignR;
    bool bPrintPlusSignI;
    bool bPrintOneR;
    bool bPrintOneI;
    bool bPaddSignR;
    bool bPaddSignI;
    int iSignLenR;
    int iSignLenI;
    int iTotalWidth;
    int iSpaceBetweenValues;
    std::wstring wstrSymbolI;
}DoubleFormat;

/*double*/
TYPES_IMPEXP void getDoubleFormat(double _dblR, double _dblI, DoubleFormat * _pDF);
TYPES_IMPEXP void addDoubleValue(std::wostringstream * _postr, double _dblValR, double _dblValI, DoubleFormat * _pDF);

/*Common*/
TYPES_IMPEXP void configureStream(std::wostringstream *_postr, int _iWidth, int _iPrec, char _cFill);
TYPES_IMPEXP void addSpaces(std::wostringstream *_postr, int _iSpace);

/*int*/
/*
  We will specify all "signed" / "unsigned" cases to avoid Compilation Warning
*/

template <typename T>
void getUnsignedIntFormat(T _TVal, int *_piWidth)
{
    if(_TVal == 0)
    {
        *_piWidth = 0;
    }
    else
    {
        *_piWidth = static_cast<int>(log10(static_cast<long double>(_TVal)) + 1);
    }
    *_piWidth += 1;
}

template <typename T>
void getSignedIntFormat(T _TVal, int *_piWidth)
{
    if(_TVal == 0)
    {
        *_piWidth = 0;
    }
    else
    {
        *_piWidth = static_cast<int>(log10(static_cast<long double>(_abs64(_TVal))) + 1);
    }
    *_piWidth += 1;
}

template <typename T>
void addUnsignedIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign = false, bool bPrintOne = true)
{
    wchar_t* pwstSign = NULL;
    wchar_t pwstFormat[32];
    wchar_t pwstOutput[32];
	if(bPrintPlusSign == true)
	{
		pwstSign = PLUS_STRING;
	}
	else
	{
		pwstSign = NO_SIGN;
	}

	if(bPrintOne == true || _TVal != 1)
	{
        os_swprintf(pwstFormat, 32, L" %ls%ld", pwstSign, _abs64(_TVal));
        os_swprintf(pwstOutput, 32, L"%*ls", _iWidth + 1, pwstFormat);//+1 for blank
        *_postr << pwstOutput;
	}
}

template <typename T>
void addSignedIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign = false, bool bPrintOne = true)
{
    const wchar_t* pwstSign = NULL;
    wchar_t pwstFormat[32];
    wchar_t pwstOutput[32];
	if(bPrintPlusSign == true)
	{
		pwstSign = (_TVal < 0 ? L"-" : L"+");
	}
	else
	{
		pwstSign = (_TVal < 0 ? L"-" : L" ");
	}

	if(bPrintOne == true || _TVal != 1)
	{
        os_swprintf(pwstFormat, 32, L" %ls%ld", pwstSign, _abs64(_TVal));
        os_swprintf(pwstOutput, 32, L"%*ls", _iWidth + 1, pwstFormat);//+1 for blank
        *_postr << pwstOutput;
	}
}


#endif /* __TOSTRING_COMMON_HXX__ */
