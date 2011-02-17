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


#ifndef __ARRAYOF_HXX__
    #error This file must only be include by arrayof.hxx
#endif

#include <vector>

namespace types
{
    class Cell : public ArrayOf<InternalType*>
    {
    public :
                            ~Cell();
                            Cell();
                            Cell(int _iRows, int _iCols);
                        	Cell(int _iDims, int* _piDims);

    private :
                            Cell(Cell* _oCellCopyMe);

    public :

        void                whoAmI(void) { std::cout << "types::Cell"; };

        RealType            getType(void) { return RealCell; }
        bool                isCell() { return true; }
        bool                isEmpty();


        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        InternalType*       clone();

        bool                set(int _iRows, int _iCols, InternalType* _pIT);
        bool                set(int _iRows, int _iCols, const InternalType* _pIT);
        bool                set(int _iIndex, InternalType* _pIT);
        bool                set(int _iIndex, const InternalType* _pIT);
        bool                set(InternalType** _pIT);

        bool                append(int _iRows, int _iCols, Cell *_poSource);

        bool                operator==(const InternalType& it);
        bool                operator!=(const InternalType& it);

        Cell*               insertCell(typed_list* _pArgs, InternalType* _pSource);
        static Cell*        insertNewCell(typed_list* _pArgs, InternalType* _pSource);
        List*               extractCell(typed_list* _pArgs);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring getTypeStr() {return L"cell";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring getShortTypeStr() {return L"ce";};
        virtual bool        isContainer(void) { return true; }
        void                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen);

    private :
        virtual InternalType*   getNullValue();
        virtual Cell*           createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
        virtual InternalType*   copyValue(InternalType* _pData);
        virtual void            deleteAll();
        virtual void            deleteImg();

    };
}
