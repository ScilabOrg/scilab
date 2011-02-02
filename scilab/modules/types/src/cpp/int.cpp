/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <sstream>
#include "arrayof.hxx"
#include "tostring_common.hxx"
#include "core_math.h"

using namespace std;

namespace types
{
	Int* Int::createInt(int _iRows, int _iCols, IntType _iIntType)
	{
		Int* pOut	= NULL;
		switch(_iIntType)
		{
		case IntType8 :
			pOut = new Int8(_iRows, _iCols);
			break;
		case TypeUnsigned8 :
			pOut = new UInt8(_iRows, _iCols);
			break;
		case Type16 :
			pOut = new Int16(_iRows, _iCols);
			break;
		case TypeUnsigned16 :
			pOut = new UInt16(_iRows, _iCols);
			break;
		case Type32 :
			pOut = new Int32(_iRows, _iCols);
			break;
		case TypeUnsigned32 :
			pOut = new UInt32(_iRows, _iCols);
			break;
		case Type64 :
			pOut = new Int64(_iRows, _iCols);
			break;
		case TypeUnsigned64 :
			pOut = new UInt64(_iRows, _iCols);
			break;
		default : 
			break;
		}
		return pOut;
	}

	bool Int::extract_getSize(int* _piMaxDim, int* _piDimSize, bool _bAsVector, int* _piRows, int* _piCols)
	{
		//check input param
		if(	(_bAsVector && _piMaxDim[0] > getSize()) ||
			(_bAsVector == false && _piMaxDim[0] > getRows()) ||
			(_bAsVector == false && _piMaxDim[1] > getCols()))
		{
			return false;
		}

		if(_bAsVector)
		{//a([])
			if(getRows() == 1)
			{
				*_piRows	= 1;
				*_piCols	= _piDimSize[0];
			}
			else
			{
				*_piRows	= _piDimSize[0];
				*_piCols	= 1;
			}
		}
		else
		{//a([],[])
			*_piRows		= _piDimSize[0];
			*_piCols		= _piDimSize[1];
		}
		return true;
	}

}
/**/
