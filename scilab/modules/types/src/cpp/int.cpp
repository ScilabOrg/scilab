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
#include "int.hxx"
#include "intt.hxx"
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
		case Type8 :
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

	InternalType::RealType Int::getType(void)
	{
		return RealInt;
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

	Int* Int::insertNew(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, Int* _poSource, bool _bAsVector)
	{
		Int* pi	= NULL ; 

		if(_bAsVector)
		{
			if(_poSource->getCols() == 1)
			{
				pi = Int::createInt(_piMaxDim[0], 1, _poSource->getIntType());
			}
			else if(_poSource->getRows() == 1)
			{
				pi = Int::createInt(1, _piMaxDim[0], _poSource->getIntType());
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			pi = Int::createInt(_piMaxDim[0], _piMaxDim[1], _poSource->getIntType());
		}

		//pi->setZeros();
		if(pi->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _iDims, _poSource, _bAsVector) == false)
		{
			delete pi;
			return NULL;
		}

		return pi;
	}

	InternalType* Int::insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, GenericType* _poSource, bool _bAsVector)
	{
		int iNewRows = getRows();
		int iNewCols = getCols();
		//check input size
		if(_bAsVector == false)
		{
			if(getRows() < _piMaxDim[0] || getCols() < _piMaxDim[1])
			{//compute new dimensions
				iNewRows = Max(_piMaxDim[0], getRows());
				iNewCols = Max(_piMaxDim[1], getCols());
			}
		}
		else
		{
			if(getSize() < _piMaxDim[0])
			{
				if(getRows() == 1 || getSize() == 0)
				{
					iNewRows = 1;
					iNewCols = _piMaxDim[0];
				}
				else if(getCols() == 1)
				{
					iNewRows = _piMaxDim[0];
					iNewCols = 1;
				}
				else
				{
					return NULL;
				}
			}
		}

		//check if the size of _poSource is compatible with the size of the variable
		if(_bAsVector == false && (iNewRows < _poSource->getRows() || iNewCols < _poSource->getCols()))
		{
			return NULL;
		}
		else if(_bAsVector == true && (iNewRows * iNewCols < _poSource->getSize()))
		{
			return NULL;
		}


		//check if the count of values is compatible with indexes
		if(_poSource->getSize() != 1 && _poSource->getSize() != _iSeqCount)
		{
			return NULL;
		}

		if(_poSource->getType() != InternalType::RealInt)
		{//TODO Yasp: overload !
			return NULL;
		}
		return this;
	}

    bool Int::append(int _iRows, int _iCols, Int *_poSource)
    {
        int iRows = _poSource->getRows();
        int iCols = _poSource->getCols();

        //insert without resize
        if(iRows + _iRows > m_iRows || iCols + _iCols > m_iCols)
        {
            return false;
        }

        for(int i = 0 ; i < iRows ; i++)
        {
            for(int j = 0 ; j < iCols ; j++)
            {
                data_set(_iRows + i, _iCols + j, _poSource->data_get(i,j));
            }
        }

        return true;
    }

}
/**/
