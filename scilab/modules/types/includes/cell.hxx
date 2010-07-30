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

#include "container.hxx"
#include "symbol.hxx"
#include <iostream>
namespace types
{
    class Cell : public Container
    {
    public :
                            Cell();
                            Cell(int _iRows, int _iCols);
                            ~Cell();

    private :
                            Cell(Cell const& _oCellCopyMe);
        void                createCell(int _iRows, int _iCols);

    public :
        int                 size_get()const;

        void                whoAmI(void) const{ std::cout << "types::Cell"; };

        RealType            getType(void) const { return RealCell; }

        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        Cell*               clone()const;

        std::string         toString(int _iPrecision, int _iLineLen) const;

        Cell*               getAsCell(void) {             std::cerr<<"real fucking cell (const)\n";return this; }
        Cell const*               getAsCell(void)const {
            std::cerr<<"real fucking cell (const)\n"; return this; }

        InternalType const*       get(int _iRows, int _iCols) const;
        InternalType const*       get(int _iIndex) const;
        InternalType *       get(int _iRows, int _iCols);
        InternalType *       get(int _iIndex);
        bool                set(int _iRows, int _iCols, InternalType const* _pIT);
        bool                set(int _iIndex, InternalType const* _pIT);


        bool                resize(int _iNewRows, int _iNewCols);
        bool                append(int _iRows, int _iCols, Cell *_poSource);

        bool                operator==(const InternalType& it) const;
        bool                operator!=(const InternalType& it) const;

        bool                insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        static Cell*               insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Double* _poSource, bool _bAsVector);
        Cell*               extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector) const;
        InternalType*       extract_cell(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string getTypeStr() const{return string("cell");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string getShortTypeStr() const{return string("ce");};

    private :
        InternalType const**      m_plData;
    };
}

#endif /* !__CELL_HXX__ */
