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

#ifndef __MATRIX_MULT__
#define __MATRIX_MULT__

#include "dynlib_ast.h"

AST_IMPEXP int iMultiComplexMatrixByComplexMatrix(
    double *_pdblReal1,		double *_pdblImg1, int _iRows1, int _iCols1,
    double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);

AST_IMPEXP int iMultiRealMatrixByRealMatrix(
    double *_pdblReal1,	int _iRows1, int _iCols1,
    double *_pdblReal2,	int _iRows2, int _iCols2,
    double *_pdblRealOut);

AST_IMPEXP int iMultiRealMatrixByComplexMatrix(
    double *_pdblReal1,		int _iRows1, int _iCols1,
    double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);

AST_IMPEXP int iMultiComplexMatrixByRealMatrix(
    double *_pdblReal1,		double *_pdblImg1,	int _iRows1, int _iCols1,
    double *_pdblReal2,		int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);

AST_IMPEXP int iMultiRealScalarByRealMatrix(
    double _dblReal1,
    double *_pdblReal2,	int _iRows2, int _iCols2,
    double *_pdblRealOut);

AST_IMPEXP int iMultiRealScalarByComplexMatrix(
    double _dblReal1,
    double *_pdblReal2,	double *_pdblImg2, int _iRows2, int _iCols2,
    double *_pdblRealOut, double *_pdblImgOut);

AST_IMPEXP int iMultiComplexScalarByRealMatrix(
    double _dblReal1,		double _dblImg1,
    double *_pdblReal2,		int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);

AST_IMPEXP int iMultiComplexScalarByComplexMatrix(
    double _dblReal1,		double _dblImg1,
    double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);


/*polynomial multiplications*/
AST_IMPEXP int iMultiScilabPolynomByScilabPolynom(
    double *_pdblReal1,	int _iRank1,
    double *_pdblReal2,	int _iRank2,
    double *_pdblRealOut, int _iRankOut);

AST_IMPEXP int iMultiComplexPolyByScilabPolynom(
    double *_pdblReal1,	double *_pdblImg1, int _iRank1,
    double *_pdblReal2,	int _iRank2,
    double *_pdblRealOut, double *_pdblImgOut, int _iRankOut);

AST_IMPEXP int iMultiScilabPolynomByComplexPoly(
    double *_pdblReal1,	int _iRank1,
    double *_pdblReal2,	double *_pdblImg2, int _iRank2,
    double *_pdblRealOut, double *_pdblImgOut, int _iRankOut);

AST_IMPEXP int iMultiComplexPolyByComplexPoly(
    double *_pdblReal1,	double *_pdblImg1, int _iRank1,
    double *_pdblReal2,	double *_pdblImg2, int _iRank2,
    double *_pdblRealOut, double *_pdblImgOut, int _iRankOut);

AST_IMPEXP int iDotMultiplyRealMatrixByRealMatrix(
    double* _pdblReal1,
    double* _pdblReal2,
    double* _pdblRealOut, int _iRowsOut, int _iColsOut);

AST_IMPEXP int iDotMultiplyRealMatrixByComplexMatrix(
    double* _pdblReal1,
    double* _pdblReal2, double* _pdblImg2,
    double* _pdblRealOut, double* _pdblImgOut, int _iRowsOut, int _iColsOut);

AST_IMPEXP int iDotMultiplyComplexMatrixByRealMatrix(
    double* _pdblReal1, double* _pdblImg1,
    double* _pdblReal2,
    double* _pdblRealOut, double* _pdblImgOut, int _iRowsOut, int _iColsOut);

AST_IMPEXP int iDotMultiplyComplexMatrixByComplexMatrix(
    double* _pdblReal1, double* _pdblImg1,
    double* _pdblReal2, double* _pdblImg2,
    double* _pdblRealOut, double* _pdblImgOut, int _iRowsOut, int _iColsOut);

#endif /* __MATRIX_MULT__ */
