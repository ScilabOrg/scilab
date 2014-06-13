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

#ifndef __MATRIX_ADD_H__
#define __MATRIX_ADD_H__

#include "dynlib_operations.hxx"

EXTERN_OP int iAddRealScalarToRealMatrix(double _dblReal1, double* _pdblReal2, int* _piDims2, int _iDims2, double* _pdblOut);
EXTERN_OP int iAddRealScalarToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int* _piDims2, int _iDims2, double* _pdblRealOut, double *_pdblImgOut);
EXTERN_OP int iAddComplexScalarToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int* _piDims2, int _iDims2, double* _pdblRealOut, double *_pdblImgOut);
EXTERN_OP int iAddComplexScalarToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int* _piDims2, int _iDims2, double* _pdblRealOut, double *_pdblImgOut);
EXTERN_OP int iAddRealMatrixToRealMatrix(double* _pdblReal1, double* _pdblReal2, int* _piDims, int _iDims, double* _pdblRealOut);
EXTERN_OP int iAddRealMatrixToComplexMatrix(double* _pdblReal1, double* _pdblReal2, double* _pdblImg2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iAddComplexMatrixToComplexMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, double* _pdblImg2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iAddRealIdentityToRealMatrix(double _dblReal1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblRealOut);
EXTERN_OP int iAddRealIdentityToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iAddComplexIdentityToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iAddComplexIdentityToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut);




EXTERN_OP int iAddScilabPolynomToScilabPolynom(double* _pCoef1, int _iRank1, double* _pCoef2, int _iRank2, double* _pCoefOut, int _iRanOut);
EXTERN_OP int iAddScilabPolynomToComplexPoly(double* _pCoef1, int _iRank1, double* _pCoefR2, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut);
EXTERN_OP int iAddComplexPolyToComplexPoly(double* _pCoef1R, double* _pCoef1I, int _iRank1, double* _pCoef2R, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut);


/*tools functions*/
int GetSize(int* _piDims, int _iDims);

/* addition double */
void add_ll_D_D(double* l, double* r, int size, double* o);
void add_ll_D_DC(double* l, double* r, double* rc, int size, double* o, double* oc);
void add_ll_D_D1(double* l, double r, int size, double* o);
void add_ll_D_D1C(double* l, double r, double rc, int size, double* o, double* oc);
void add_ll_D_E(double* l, int size, double* o);

void add_ll_DC_D(double* l, double* lc, double* r, int size, double* o, double* oc);
void add_ll_DC_DC(double* l, double* lc, double* r, double* rc, int size, double* o, double* oc);
void add_ll_DC_D1(double* l, double* lc, double r, int size, double* o, double* oc);
void add_ll_DC_D1C(double* l, double* lc, double r, double rc, int size, double* o, double* oc);
void add_ll_DC_E(double* l, double* lc, int size, double* o, double* oc);

void add_ll_D1_D(double l, double* r, int size, double* o);
void add_ll_D1_DC(double l, double* r, double* rc, int size, double* o, double* oc);
void add_ll_D1_D1(double l, double r, double* o);
void add_ll_D1_D1C(double l, double r, double rc, double* o, double* oc);
void add_ll_D1_E(double l, double* o);

void add_ll_D1C_D(double l, double lc, double* r, int size, double* o, double* oc);
void add_ll_D1C_DC(double l, double lc, double* r, double* rc, int size, double* o, double* oc);
void add_ll_D1C_D1(double l, double lc, double r, double* o, double* oc);
void add_ll_D1C_D1C(double l, double lc, double r, double rc, double* o, double* oc);
void add_ll_D1C_E(double l, double lc, double* o, double* oc);

void add_ll_E_D(double* r, int size, double* o);
void add_ll_E_DC(double* r, double* rc, int size, double* o, double* oc);
void add_ll_E_D1(double r, double* o);
void add_ll_E_D1C(double r, double rc, double* o, double* oc);
void add_ll_E_E();

#endif /* __MATRIX_ADD_H__ */
