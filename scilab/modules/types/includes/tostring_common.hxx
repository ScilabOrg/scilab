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

#define SIZE_BETWEEN_TWO_VALUES         2
#define SPACE_BETWEEN_TWO_VALUES        L"  "
#define SIZE_BETWEEN_REAL_COMPLEX       1
#define SPACE_BETWEEN_REAL_COMPLEX      L" "
#define SIZE_BOOL                       1
#define SIZE_BETWEEN_BOOL               1
#define SPACE_BETWEEN_BOOL              L" "
#define SIGN_LENGTH                     2
#define NO_SIGN                         L"  "
#define MINUS_STRING                    L"- "
#define PLUS_STRING                     L"+ "
#define MINUS_STRING_INT                L" -"
#define SYMBOL_I                        L"i"

#ifndef _MSC_VER
#include <inttypes.h>
#define _abs64(x) llabs(x)
#endif

/*double*/
void getDoubleFormat(double _dblVal, int _iPrecNeeded, int *_piWidth, int *_piPrec, bool* _pbFloatingPoint);
void getComplexFormat(double _dblR, double _dblI, int _iPrecNeeded, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, int *_piPrecR,  int *_piPrecI, bool* _pbFloatingPointR,  bool* _pbFloatingPointI);
void addDoubleValue(std::wostringstream *_postr, double _dblVal, int _iWidth, int _iPrec, bool bPrintPlusSign = false, bool bPrintOne = true, bool bPaddSign = true);
void addDoubleComplexValue(std::wostringstream *_postr, double _dblR, double _dblI, int _iTotalWitdh, int _iWidthR, int _iWidthI, int _iPrec);
void printDoubleVar(std::wostringstream *_postr,  double _dblVal);

/*Common*/
void configureStream(std::wostringstream *_postr, int _iWidth, int _iPrec, char _cFill);
void addSpaces(std::wostringstream *_postr, int _iSpace);

/*int*/
/*
  We will specify all "signed" / "unsigned" cases to avoid Compilation Warning
*/

template <typename T>
void getUnsignedIntFormat(T _TVal, int *_piWidth)
{
    *_piWidth = static_cast<int>(log10(static_cast<long double>(_TVal)) + 1);
}

template <typename T>
void getSignedIntFormat(T _TVal, int *_piWidth)
{
    *_piWidth = static_cast<int>(log10(static_cast<long double>(_abs64(_TVal))) + 1);
}

template <typename T>
void addUnsignedIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign = false, bool bPrintOne = true)
{
	if(bPrintPlusSign == true)
	{
		*_postr << PLUS_STRING;
	}
	else
	{
		*_postr << NO_SIGN;
	}

	configureStream(_postr, _iWidth, 0, ' ');

	if(bPrintOne == true || _TVal != 1)
	{
        *_postr << std::right << static_cast<unsigned long long>(_TVal);
	}
}

template <typename T>
void addSignedIntValue(std::wostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign = false, bool bPrintOne = true)
{
	if(bPrintPlusSign == true)
	{
		*_postr << (_TVal < 0 ? MINUS_STRING_INT : PLUS_STRING);
	}
	else
	{
		*_postr << (_TVal < 0 ? MINUS_STRING_INT : NO_SIGN);
	}

	configureStream(_postr, _iWidth, 0, ' ');

	if(bPrintOne == true || _TVal != 1)
	{
        *_postr << std::right << static_cast<long long>(_abs64(_TVal));
	}
}


#endif /* __TOSTRING_COMMON_HXX__ */
