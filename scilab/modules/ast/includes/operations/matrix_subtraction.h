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

#ifndef __MATRIX_SUBTRACT_H__
#define __MATRIX_SUBTRACT_H__

int iSubstractRealIdentityToRealMatrix(double _dblReal1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblRealOut);
int iSubstractRealIdentityToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut);
int iSubstractComplexIdentityToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut);
int iSubstractComplexIdentityToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut);

int iSubstractRealMatrixToRealScalar(double* _pdblReal1, int* _piDims1, int _iDims1, double _dblReal2, double* _pdblRealOut);
int iSubstractComplexMatrixToRealScalar(double* _pdblReal1, double* _pdblImg1, int* _piDims1, int _iDims1, double _dblReal2, double* _pdblRealOut, double* _pdblImgOut);
int iSubstractRealMatrixToComplexScalar(double* _pdblReal1, int* _piDims1, int _iDims1, double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut);
int iSubstractComplexMatrixToComplexScalar(double* _pdblReal1, double* _pdblImg1, int* _piDims1, int _iDims1, double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut);

int iSubstractRealScalarToRealMatrix(double _dblReal1, double* _pdblReal2, int* _piDims2, int _iDims2, double* _pdblRealOut);
int iSubstractComplexScalarToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int* _piDims2, int _iDims2, double* _pdblRealOut, double* _pdblImgOut);
int iSubstractRealScalarToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int* _piDims2, int _iDims2, double* _pdblRealOut, double* _pdblImgOut);
int iSubstractComplexScalarToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int* _piDims2, int _iDims2, double* _pdblRealOut, double* _pdblImgOut);

int iSubstractRealMatrixToRealMatrix(double* _pdblReal1, double* _pdblReal2, int* _piDims, int _iDims, double* _pdblRealOut);
int iSubstractComplexMatrixToRealMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut);
int iSubstractRealMatrixToComplexMatrix(double* _pdblReal1, double* _pdblReal2, double* _pdblImg2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut);
int iSubstractComplexMatrixToComplexMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, double* _pdblImg2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut);

#endif /* __MATRIX_SUBTRACT_H__ */
