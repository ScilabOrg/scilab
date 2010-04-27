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
#include "double.hxx"
#include "tostring_common.hxx"

namespace types 
{
    /**
    ** Constructor & Destructor (public)
    */
    Cell::Cell() : Container()
    {
        m_plData = NULL;
        m_iRows = 0;
        m_iCols = 0;
        m_iSize = 0;
    }

    Cell::Cell(int _iRows, int _iCols)
    {
        createCell(_iRows, _iCols);
    }

    void Cell::createCell(int _iRows, int _iCols)
    {
        m_iRows = _iRows;
        m_iCols = _iCols;
        m_plData = new InternalType*[size_get()];
        
        Double* pEmpty = new Double(0,0);
        for(int i = 0 ; i < size_get() ; i++)
        {
            pEmpty->IncreaseRef();
            m_plData[i] = pEmpty;
        }
    }

    Cell::~Cell() 
    {
        if(isDeletable() == true)
        {
            for(int i = 0 ; i < size_get() ; i++)
            {
                m_plData[i]->DecreaseRef();
                if(m_plData[i]->isDeletable())
                {
                    delete m_plData[i];
                }
            }
            delete[] m_plData;
        }
    }

    /** 
    ** Private Copy Constructor and data Access
    */
    Cell::Cell(Cell *_oCellCopyMe)
    {
        createCell(_oCellCopyMe->rows_get(), _oCellCopyMe->cols_get());
        for(int i = 0 ; i < size_get() ; i++)
        {
            
        }
    }

    InternalType* Cell::get(int _iIndex)
    {
        if(_iIndex < size_get())
        {
            return m_plData[_iIndex];
        }
        return NULL;
    }

    InternalType* Cell::get(int _iRows, int _iCols)
    {
        if(_iRows < rows_get() && _iCols < cols_get())
        {
            return get(_iCols * rows_get() + _iRows);
        }
        return NULL;
    }

    bool Cell::set(int _iRows, int _iCols, InternalType* _pIT)
    {
        if(_iRows < rows_get() && _iCols < cols_get())
        {
            return set(_iCols * rows_get() + _iRows, _pIT);
        }
        return false;
    }
     
    bool Cell::set(int _iIndex, InternalType* _pIT)
    {
        if(_iIndex < size_get())
        {
            if(m_plData[_iIndex] != NULL)
            {
                m_plData[_iIndex]->DecreaseRef();
                if(m_plData[_iIndex]->isDeletable())
                {
                    delete m_plData[_iIndex];
                }
            }
            
            _pIT->IncreaseRef();
            m_plData[_iIndex] = _pIT;
            return true;
        }
        return false;
    }

    /**
    ** size_get
    ** Return the number of elements in struct
    */
    int Cell::size_get() 
    {
        return m_iRows * m_iCols;
    }

    /**
    ** Clone
    ** Create a new Struct and Copy all values.
    */
    Cell *Cell::clone()
    {
        return new Cell(this);
    }

    /**
    ** toString to display Structs
    ** FIXME : Find a better indentation process
    */
    std::string Cell::toString(int _iPrecision, int _iLineLen)
    {
        std::ostringstream ostr;
       
        //max len for each column
        int *piColLen = new int[rows_get()];
        int *piILen = new int[rows_get()];
        int *piJLen = new int[rows_get()];
        int *piSumLen = new int[rows_get()];

        memset(piColLen, 0x00, rows_get() * sizeof(int));
        memset(piILen,  0x00, rows_get() * sizeof(int));
        memset(piJLen, 0x00, rows_get() * sizeof(int));
        memset(piSumLen, 0x00, rows_get() * sizeof(int));

        for(int j = 0 ; j < cols_get() ; j++)
        {
            for(int i = 0 ; i < rows_get() ; i++)
            {
                InternalType* pIT = get(i,j);

                std::string strType = pIT->getTypeStr();
                if(pIT->isAssignable())
                {
                    //compute number of digits to write rows and cols
                    int iRowsLen = static_cast<int>(log10(static_cast<double>(pIT->getAsGenericType()->rows_get())) + 1);
                    int iColsLen = static_cast<int>(log10(static_cast<double>(pIT->getAsGenericType()->cols_get())) + 1);

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

        for(int i = 0 ; i < rows_get() ; i++)
        {
            for(int j = 0 ; j < cols_get() ; j++)
            {
                InternalType* pIT = get(i,j);

                ostr << "  [";
                if(pIT->isAssignable())
                {
                    std::ostringstream ostemp;
                    Config_Stream(&ostemp, piILen[j], _iPrecision, ' ');
                    ostemp << right << pIT->getAsGenericType()->rows_get();
                    ostemp << "x";
                    Config_Stream(&ostemp, piJLen[j], _iPrecision, ' ');
                    ostemp << left << pIT->getAsGenericType()->cols_get();
                    Config_Stream(&ostemp, piSumLen[j] - static_cast<int>(ostemp.str().size()), _iPrecision, ' ');
                    ostemp << "";//fill with space
                    ostr << ostemp.str();
                }
                else
                {
                    Config_Stream(&ostr, piSumLen[j], _iPrecision, ' ');
                    ostr << "";//fill with space
                }
                ostr << " ";
                Config_Stream(&ostr, piColLen[j], _iPrecision, ' ');
                ostr << left << pIT->getTypeStr();
                ostr << "]";
            }
            ostr << std::endl;
        }

        delete[] piColLen;
        delete[] piILen;
        delete[] piJLen;
        delete[] piSumLen;
        return ostr.str();
    }

    bool Cell::resize(int _iNewRows, int _iNewCols)
    {
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

		if(pC->rows_get() != rows_get() || pC->cols_get() != cols_get())
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
}
