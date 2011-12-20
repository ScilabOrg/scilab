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

// Real ./. Real
int iRDivideKronR(  double* _pdblRealIn1, int _iRowsIn1, int _iColsIn1,
                    double* _pdblRealIn2, int _iRowsIn2, int _iColsIn2,
                    double* _pdblRealOut, int _iRowsOut, int _iColsOut)
{
    int iOutSet = 1;
    int i, j, k;
    int one = 1;
    int iPosIn1;
    int iPosIn2;
    int iPosInOut;
    int iOutSize = _iRowsOut * _iColsOut;
    double* pdTemp = (double*)malloc(iOutSize * sizeof(double));
    double eps = C2F(dlamch)("p",1L);

    for(i = 0; i < _iRowsIn2; i++)
    {
        for(j = 0; j < _iColsIn2; j++)
        {
            iPosIn2 = j * _iRowsIn2 + i;
            if(_pdblRealIn2[iPosIn2] == 0) // division by 0
            {
                continue;
            }

            for(k = 0; k < _iColsOut; k++)
            {
                iPosIn1 = j * _iColsOut * _iRowsIn1 + i * _iRowsOut  + (k * _iRowsIn1);
                iPosInOut = k * _iRowsOut;
                C2F(dcopy)(&_iRowsOut, _pdblRealIn1 + iPosIn1, &one, pdTemp + iPosInOut, &one);
            }

            for(k = 0; k < iOutSize; k++)
            {
                pdTemp[k] /= _pdblRealIn2[iPosIn2];
            }

            if(iOutSet == 0)
            {
                for(k = 0; k < iOutSize; k++)
                {
                    if((fabs(pdTemp[k] - _pdblRealOut[k]) > eps) && (fabs(pdTemp[k]) > eps))
                    {
                        free(pdTemp);
                        return 2;
                    }
                }
            }
            else
            {
                C2F(dcopy)(&iOutSize, pdTemp, &one,_pdblRealOut, &one);
                iOutSet = 0;
            }
        }
    }
    free(pdTemp);
    return iOutSet;
}

// Complex ./. Complex
int iRDivideKronC(  double* _pdblRealIn1, double* _pdblImgIn1, int _iRowsIn1, int _iColsIn1,
                    double* _pdblRealIn2, double* _pdblImgIn2, int _iRowsIn2, int _iColsIn2,
                    double* _pdblRealOut, double* _pdblImgOut, int _iRowsOut, int _iColsOut)
{
    int iOutSet = 1;
    int i, j, k;
    int one = 1;
    int iPosIn1;
    int iPosIn2;
    int iPosInOut;
    int iOutSize = _iRowsOut * _iColsOut;
    double* pdTemp = (double*)malloc(iOutSize * sizeof(double));
    double* pdTempi = (double*)malloc(iOutSize * sizeof(double));
    double eps = C2F(dlamch)("p",1L);

    for(i = 0; i < _iRowsIn2; i++)
    {
        for(j = 0; j < _iColsIn2; j++)
        {
            iPosIn2 = j * _iRowsIn2 + i;
            if(_pdblRealIn2[iPosIn2] == 0 && _pdblImgIn2[iPosIn2] == 0) // division by 0
            {
                continue;
            }

            for(k = 0; k < _iColsOut; k++)
            {
                iPosIn1 = j * _iColsOut * _iRowsIn1 + i * _iRowsOut  + (k * _iRowsIn1);
                iPosInOut = k * _iRowsOut;
                C2F(dcopy)(&_iRowsOut, _pdblRealIn1 + iPosIn1, &one, pdTemp + iPosInOut, &one);
                C2F(dcopy)(&_iRowsOut, _pdblImgIn1 + iPosIn1, &one, pdTempi + iPosInOut, &one);
            }

            for(k = 0; k < iOutSize; k++)
            {
                double temp =   _pdblRealIn2[iPosIn2] * _pdblRealIn2[iPosIn2] +
                                _pdblImgIn2[iPosIn2] * _pdblImgIn2[iPosIn2];

                double pdTempK = pdTemp[k] * _pdblRealIn2[iPosIn2] + pdTempi[k] * _pdblImgIn2[iPosIn2];
                double pdTempiK = pdTempi[k] * _pdblRealIn2[iPosIn2] - pdTemp[k] * _pdblImgIn2[iPosIn2];

                pdTemp[k] = pdTempK / temp;
                pdTempi[k] = pdTempiK / temp;
            }

            if(iOutSet == 0)
            {
                for(k = 0; k < iOutSize; k++)
                {
                    if( ((fabs(pdTemp[k] - _pdblRealOut[k]) > eps) && (fabs(pdTemp[k]) > eps)) || 
                        ((fabs(pdTempi[k] - _pdblImgOut[k]) > eps) && (fabs(pdTempi[k]) > eps)))
                    {
                        free(pdTempi);
                        free(pdTemp);
                        return 2;
                    }
                }
            }
            else
            {
                C2F(dcopy)(&iOutSize, pdTemp, &one,_pdblRealOut, &one);
                C2F(dcopy)(&iOutSize, pdTempi, &one,_pdblImgOut, &one);
                iOutSet = 0;
            }
        }
    }
    free(pdTempi);
    free(pdTemp);
    return iOutSet;
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

