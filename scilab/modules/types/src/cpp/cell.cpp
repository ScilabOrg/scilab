/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <sstream>
#include <math.h>
#include "symbol.hxx"
#include "cell.hxx"
#include "arrayof.hxx"
#include "tostring_common.hxx"
#include "core_math.h"

namespace types
{
    /**
    ** Constructor & Destructor (public)
    */
    Cell::Cell() : Container()
    {
        createCell(0, 0);
    }

    Cell::Cell(int _iRows, int _iCols)
    {
        int iRows = Max(0, _iRows);
        int iCols = Max(0, _iCols);
        createCell(iRows, iCols);
    }

    void Cell::createCell(int _iRows, int _iCols)
    {
        m_iRows = _iRows;
        m_iCols = _iCols;
        m_iSize = m_iRows * m_iCols;
        m_iSizeMax = m_iSize;

        m_vectData = new std::vector<InternalType*>;
        if(m_iSize != 0)
        {
            m_vectData->resize(getSize());
        }

        Double* pEmpty = Double::Empty();
        for(int i = 0 ; i < getSize() ; i++)
        {
            pEmpty->IncreaseRef();
            (*m_vectData)[i] = pEmpty;
        }
    }

    Cell::~Cell()
    {
        if(isDeletable() == true)
        {
            for(int i = 0 ; i < getSize() ; i++)
            {
                (*m_vectData)[i]->DecreaseRef();
                if((*m_vectData)[i]->isDeletable())
                {
                    delete (*m_vectData)[i];
                }
            }
        }
    }

    /**
    ** Private Copy Constructor and data Access
    */
    Cell::Cell(Cell *_oCellCopyMe)
    {
        createCell(_oCellCopyMe->getRows(), _oCellCopyMe->getCols());
        for(int i = 0 ; i < getSize() ; i++)
        {
            set(i, _oCellCopyMe->get(i));
        }
    }

    InternalType* Cell::get(int _iIndex)
    {
        if(_iIndex < getSize())
        {
            return (*m_vectData)[_iIndex];
        }
        return NULL;
    }

    InternalType* Cell::get(int _iRows, int _iCols)
    {
        if(_iRows < getRows() && _iCols < getCols())
        {
            return get(_iCols * getRows() + _iRows);
        }
        return NULL;
    }

    bool Cell::set(int _iRows, int _iCols, InternalType* _pIT)
    {
        if(_iRows < getRows() && _iCols < getCols())
        {
            return set(_iCols * getRows() + _iRows, _pIT);
        }
        return false;
    }

    bool Cell::set(int _iIndex, InternalType* _pIT)
    {
        if(_iIndex < getSize())
        {
            if((*m_vectData)[_iIndex] != NULL)
            {
                (*m_vectData)[_iIndex]->DecreaseRef();
                if((*m_vectData)[_iIndex]->isDeletable())
                {
                    delete (*m_vectData)[_iIndex];
                }
            }

            _pIT->IncreaseRef();
            (*m_vectData)[_iIndex] = _pIT;
            return true;
        }
        return false;
    }

    /**
    ** size_get
    ** Return the number of elements in struct
    */
    int Cell::getSize()
    {
        return m_iSize;
    }

    /**
    ** Clone
    ** Create a new Struct and Copy all values.
    */
    InternalType *Cell::clone()
    {
        return new Cell(this);
    }

    GenericType* Cell::getColumnValues(int _iPos)
    {
		Cell *pCell = NULL;
		if(_iPos < m_iCols)
		{
			pCell = new Cell(m_iRows, 1);
			for(int i = 0 ; i < m_iRows ; i++)
			{
				pCell->set(i, 0, get(i, _iPos));
			}
		}
		return pCell;
    }

    /**
    ** toString to display Structs
    ** FIXME : Find a better indentation process
    */
    std::wstring Cell::toString(int _iPrecision, int _iLineLen)
    {
        std::wostringstream ostr;

        if(getSize() == 0)
        {
            ostr << L"   {}";
        }
        else
        {
            //max len for each column
            int *piColLen = new int[getCols()];
            int *piILen = new int[getCols()];
            int *piJLen = new int[getCols()];
            int *piSumLen = new int[getCols()];

            memset(piColLen, 0x00, getCols() * sizeof(int));
            memset(piILen,  0x00, getCols() * sizeof(int));
            memset(piJLen, 0x00, getCols() * sizeof(int));
            memset(piSumLen, 0x00, getCols() * sizeof(int));

            for(int j = 0 ; j < getCols() ; j++)
            {
                for(int i = 0 ; i < getRows() ; i++)
                {
                    InternalType* pIT = get(i,j);

                    std::wstring strType = pIT->getTypeStr();
                    if(pIT->isAssignable())
                    {
                        //compute number of digits to write rows and cols
                        int iRowsLen = static_cast<int>(log10(static_cast<double>(pIT->getAsGenericType()->getRows())) + 1);
                        int iColsLen = static_cast<int>(log10(static_cast<double>(pIT->getAsGenericType()->getCols())) + 1);

                        if(piILen[j] < iRowsLen)
                        {
                            piILen[j] = iRowsLen;
                        }

                        if(piJLen[j] < iColsLen)
                        {
                            piJLen[j] = iColsLen;
                        }

                        if(piSumLen[j] < (iColsLen + iRowsLen + 1))
                        {
                            piSumLen[j] = iColsLen + iRowsLen + 1; //+1 for 'x' character
                        }
                    }

                    if(piColLen[j] < strType.size())
                    {
                        piColLen[j] = static_cast<int>(strType.size());
                    }
                }
            }

            for(int i = 0 ; i < getRows() ; i++)
            {
                for(int j = 0 ; j < getCols() ; j++)
                {
                    InternalType* pIT = get(i,j);

                    ostr << L"  [";
                    if(pIT->isAssignable())
                    {
                        std::wostringstream ostemp;
                        configureStream(&ostemp, piILen[j], _iPrecision, ' ');
                        ostemp << std::right << pIT->getAsGenericType()->getRows();
                        ostemp << L"x";
                        configureStream(&ostemp, piJLen[j], _iPrecision, ' ');
                        ostemp << std::left << pIT->getAsGenericType()->getCols();
                        configureStream(&ostemp, piSumLen[j] - static_cast<int>(ostemp.str().size()), _iPrecision, ' ');
                        ostemp << L"";//fill with space
                        ostr << ostemp.str();
                    }
                    else
                    {
                        configureStream(&ostr, piSumLen[j], _iPrecision, ' ');
                        ostr << L"";//fill with space
                    }
                    ostr << L" ";
                    configureStream(&ostr, piColLen[j], _iPrecision, ' ');
                    ostr << std::left << pIT->getTypeStr();
                    ostr << L"]";
                }
                ostr << std::endl;
            }

            delete[] piColLen;
            delete[] piILen;
            delete[] piJLen;
            delete[] piSumLen;
        }
        ostr << std::endl;
        return ostr.str();
    }

    bool Cell::resize(int _iNewRows, int _iNewCols)
    {
        if(_iNewRows <= getRows() && _iNewCols <= getCols())
        {//nothing to do
            return true;
        }

        if(m_iSizeMax < _iNewRows * _iNewCols)
        {
            //alloc 10% bigger than asked to prevent future resize
            m_iSizeMax = static_cast<int>(_iNewRows * _iNewCols * 1.1);

            //alloc new data array
            std::vector<InternalType*>* pIT = NULL;

            pIT = new std::vector<InternalType*>;
            pIT->resize(m_iSizeMax);

            for(int i = 0 ; i < _iNewRows ; i++)
            {
                for(int j = 0 ; j < _iNewCols ; j++)
                {
                    (*pIT)[j * _iNewRows + i] = Double::Empty();
                }
            }

            //copy existing values
            for(int i = 0 ; i < getRows() ; i++)
            {
                for(int j = 0 ; j < getCols() ; j++)
                {
                    delete (*pIT)[j * _iNewRows + i];
                    (*pIT)[j * _iNewRows + i] = (*m_vectData)[j * getRows() + i];
                }
            }
            delete m_vectData;
            m_vectData	= pIT;

        }

        m_iRows = _iNewRows;
        m_iCols	= _iNewCols;
        m_iSize = m_iRows * m_iCols;
        return true;
    }

    bool Cell::append(int _iRows, int _iCols, Cell *_poSource)
    {
        return true;
    }

    bool Cell::operator==(const InternalType& it)
    {
        if(const_cast<InternalType &>(it).getType() != RealCell)
        {
            return false;
        }

        Cell* pC = const_cast<InternalType &>(it).getAsCell();

        if(pC->getRows() != getRows() || pC->getCols() != getCols())
        {
            return false;
        }

        for(int i = 0 ; i < m_iRows ; i++)
        {
            for(int j = 0 ; j < m_iCols ; j++)
            {
                if(get(i,j) != pC->get(i,j))
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool Cell::operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    Cell* Cell::extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, int* _piDimSize, bool _bAsVector)
    {
        Cell* pOut		= NULL;
        int iRowsOut	= 0;
        int iColsOut	= 0;

        //check input param

        if(	(_bAsVector && _piMaxDim[0] > getSize()) ||
            (_bAsVector == false && _piMaxDim[0] > getRows()) ||
            (_bAsVector == false && _piMaxDim[1] > getCols()))
        {
            return NULL;
        }

        if(_bAsVector)
        {//a([])
            if(getRows() == 1)
            {
                iRowsOut	= 1;
                iColsOut	= _piDimSize[0];
            }
            else
            {
                iRowsOut	= _piDimSize[0];
                iColsOut	= 1;
            }
        }
        else
        {//a([],[])
            iRowsOut		= _piDimSize[0];
            iColsOut		= _piDimSize[1];
        }

        pOut					= new Cell(iRowsOut, iColsOut);


        if(_bAsVector)
        {
            for(int i = 0 ; i < _iSeqCount ; i++)
            {
                pOut->set(i, get(_piSeqCoord[i] - 1));
            }
        }
        else
        {
            for(int i = 0 ; i < _iSeqCount ; i++)
            {
                //convert vertical indexes to horizontal indexes
                int iCurIndex		= (i % iColsOut) * iRowsOut + (i / iColsOut);
                int iInIndex		= (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * getRows();
                pOut->set(iCurIndex, get(iInIndex));
            }
        }
        return pOut;
    }

    std::vector<InternalType*> Cell::extractCell(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
    {
        std::vector<InternalType*> vectRet;

        //check input param
        if(	(_bAsVector && _piMaxDim[0] > getSize()) ||
            (_bAsVector == false && _piMaxDim[0] > getRows()) ||
            (_bAsVector == false && _piMaxDim[1] > getCols()))
        {
            return vectRet;
        }

        if(_bAsVector)
        {
            for(int i = 0 ; i < _iSeqCount ; i++)
            {
                vectRet.push_back((*m_vectData)[_piSeqCoord[i] - 1]);
            }
        }
        else
        {
            for(int i = 0 ; i < _iSeqCount ; i++)
            {
                //convert vertical indexes to horizontal indexes
                int iInIndex = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * getRows();
                vectRet.push_back((*m_vectData)[iInIndex]);
            }
        }

        return vectRet;
    }

    InternalType* Cell::insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, GenericType* _poSource, bool _bAsVector)
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


        if(_poSource->isCell())
        {
            Cell *pIn = _poSource->getAsCell();

            //Only resize after all tests !
            if(resize(iNewRows, iNewCols) == false)
            {
                return NULL;
            }

            ////variable can receive new values.
            if(pIn->getSize() == 1)
            {//a(?) = x
                if(_bAsVector)
                {//a([]) = R
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        set(_piSeqCoord[i] - 1, pIn->get(0));
                    }
                }
                else
                {//a([],[]) = R
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * getRows();
                        set(iPos, pIn->get(0));
                    }
                }
            }
            else
            {//a(?) = [x]
                if(_bAsVector)
                {//a([]) = [R]
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        set(_piSeqCoord[i] - 1, pIn->get(i));
                    }
                }
                else
                {//a([],[]) = [R]
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * getRows();
                        int iTempR = i / pIn->getCols();
                        int iTempC = i % pIn->getCols();
                        int iNew_i = iTempR + iTempC * pIn->getRows();

                        set(iPos, pIn->get(iNew_i));
                    }
                }
            }
        }
        else
        {
            return NULL;
        }
        return this;
    }

    Cell* Cell::insertNew(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, GenericType* _poSource, bool _bAsVector)
    {
        Cell *pCell = NULL;

        if(_bAsVector)
        {
            pCell = new Cell(1, _piMaxDim[0]);
        }
        else
        {
            pCell = new Cell(_piMaxDim[0], _piMaxDim[1]);
        }

        if(pCell->insertCell(_iSeqCount, _piSeqCoord, _piMaxDim, _iDims, _poSource, _bAsVector) == false)
        {
            delete pCell;
            return NULL;
        }

        return pCell;
    }

    bool Cell::insertCell(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, GenericType* _poSource, bool _bAsVector)
    {
        int iNewRows = getRows();
        int iNewCols = getCols();
        //check input size
        if(_iSeqCount != 1)
        {
            return false;
        }


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
                    return false;
                }
            }
        }

        //Only resize after all tests !
        if(resize(iNewRows, iNewCols) == false)
        {
            return false;
        }

        if(_bAsVector)
        {//a{[]} = R
            set(_piSeqCoord[0] - 1, _poSource);
        }
        else
        {//a([],[]) = R
            int iPos = (_piSeqCoord[0] - 1) + (_piSeqCoord[1] - 1) * getRows();
            set(iPos, _poSource);
        }
        return true;
    }
}
