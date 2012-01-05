/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine Elias
*  Copyright (C) 2011 - DIGITEO - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "matrix_kronecker.h"
#include "elem_common.h"

// Real .*. Real
void vKronR(double* _pdblDataIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
            double* _pdblDataIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
            double* _pdblDataOut, int _iIncOut)
{
    int iOne    = 1;
    int iLoop1  = 0;
    int iLoop2  = 0;
    int iLoop3  = 0;
    int iIndex1 = - _iIncIn1;
    int iIndex2 = - _iColsIn2;

    for(iLoop1 = 0; iLoop1 < _iColsIn1; iLoop1++)
    {
        int iIndex3 = 0; //Index a 1 ???
        iIndex1     += _iIncIn1;
        iIndex2     += _iColsIn2;
        for(iLoop2 = 0; iLoop2 < _iColsIn2; iLoop2++)
        {
            int iIndex4 = iIndex1;
            int iIndex5 = (iLoop2 + iIndex2) * _iIncOut;
            for(iLoop3 = 0; iLoop3 < _iRowsIn1; iLoop3++)
            {
                C2F(dcopy)(&_iRowsIn2, &_pdblDataIn2[iIndex3], &iOne, &_pdblDataOut[iIndex5], &iOne);
                //ddscals(&_pdblDataOut[iIndex5], _iRowsIn2, _pdblDataIn1[iIndex4], &_pdblDataOut[iIndex5]);
                C2F(dscal)(&_iRowsIn2, &_pdblDataIn1[iIndex4], &_pdblDataOut[iIndex5], &iOne);

                iIndex5 += _iRowsIn2;
                iIndex4++;
            }
            iIndex3 += _iIncIn2;
        }
    }
}

// Complex .*. Complex
void vKronC(double* _pdblRealIn1, double* _pdblImgIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
            double* _pdblRealIn2, double* _pdblImgIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
            double* _pdblRealOut, double* _pdblImgOut, int _iIncOut)
{
    int iLoop1 = 0, iLoop2 = 0, iLoop3 = 0, iLoop4 = 0;
    int iIndex1 = -_iIncIn1;
    int iIndex2 = -_iColsIn2;
    for(iLoop1 = 0; iLoop1 < _iColsIn1; iLoop1++)
    {
        int iIndex3 = 0;
        iIndex1     += _iIncIn1;
        iIndex2     += _iColsIn2;
        for(iLoop2 = 0; iLoop2 < _iColsIn2; iLoop2++)
        {
            int iIndex4 = iIndex1;
            int iIndex5 = (iLoop2 + iIndex2) * _iIncOut;
            for(iLoop3 = 0; iLoop3 < _iRowsIn1; iLoop3++)
            {
                for(iLoop4 = 0; iLoop4 < _iRowsIn2; iLoop4++)
                {
                    _pdblRealOut[iIndex5 + iLoop4] =
                        _pdblRealIn1[iIndex4] * _pdblRealIn2[iIndex3 + iLoop4] -
                        _pdblImgIn1[iIndex4] * _pdblImgIn2[iIndex3 + iLoop4];
                    _pdblImgOut[iIndex5 + iLoop4] =
                        _pdblRealIn1[iIndex4] * _pdblImgIn2[iIndex3 + iLoop4] +
                        _pdblImgIn1[iIndex4] * _pdblRealIn2[iIndex3 + iLoop4];
                }
                iIndex5 += _iRowsIn2;
                iIndex4++;
            }
            iIndex3 += _iIncIn2;
        }
    }
}

// convert : a => 1 ./ a
int conv_real_input(double* _pdblData, int _iSize)
{
    int i;
    for(i = 0 ; i < _iSize ; i++)
    {
        if(_pdblData[i] != 0)
        {
            _pdblData[i] = 1.0 / _pdblData[i];
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

int conv_img_input(double* _pdblReal, double* _pdblImg, int _iSize)
{
    int i;
    for(i = 0 ; i < _iSize ; i++)
    {
        double dblR = _pdblReal[i];
        double dblI = _pdblImg[i];

        double dblTemp	= dblR * dblR + dblI * dblI;
        if(dblTemp != 0)
        {
            _pdblReal[i] = _pdblReal[i]	/ dblTemp;
            _pdblImg[i] = - _pdblImg[i] / dblTemp;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

