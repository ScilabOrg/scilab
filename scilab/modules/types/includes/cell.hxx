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


#ifndef __CELL_HXX__
#define __CELL_HXX__

#include <vector>
#include "container.hxx"

namespace types
{
    class Cell : public Container
    {
    public :
                            Cell();
                            Cell(int _iRows, int _iCols);
                            ~Cell();

    private :
                            Cell(Cell* _oCellCopyMe);
        void                createCell(int _iRows, int _iCols);

    public :
        int                 getSize();

        void                whoAmI(void) { std::cout << "types::Cell"; };

        RealType            getType(void) { return RealCell; }
        bool                isCell() { return true; }

        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        InternalType*       clone();

        std::wstring        toString(int _iPrecision, int _iLineLen);

        Cell*               getAsCell(void) { return this; }

        InternalType*       get(int _iRows, int _iCols);
        InternalType*       get(int _iIndex);
        bool                set(int _iRows, int _iCols, InternalType* _pIT);
        bool                set(int _iIndex, InternalType* _pIT);

        GenericType*        getColumnValues(int _iPos);


        bool                resize(int _iNewRows, int _iNewCols);
        bool                append(int _iRows, int _iCols, Cell *_poSource);

        bool                operator==(const InternalType& it);
        bool                operator!=(const InternalType& it);

        InternalType*       insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, GenericType* _poSource, bool _bAsVector);
        bool                insertCell(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, GenericType* _poSource, bool _bAsVector);
        static Cell*        insertNew(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, GenericType* _poSource, bool _bAsVector);
        Cell*               extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, int* _piDimSize, bool _bAsVector);

        std::vector<InternalType*> extractCell(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring getTypeStr() {return L"cell";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring getShortTypeStr() {return L"ce";};

    private :
        std::vector<InternalType*>*   m_vectData;
    };
}

#endif /* !__CELL_HXX__ */
