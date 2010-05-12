/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef EIGEN_H
#define EIGEN_H

#include "doublecomplex.h"

/*
 * Functions used to compute eigen values (and possibly eigen vectors) of one [complex|real] [un]symmetric matrix.
 * We use 4 different functions because for e;fficiency reasons, API had to be different :
 * 1) eigen vectors for symmetric matrix are computed in place in the same memory location as the argument matrix
 * 2) complex eigen values for unsymmetric matrix are computed either in 'c' format (for real argument matrix) or 'z' format (complex argument matrix)
 *
 * @param pData double[complex]* in (& out for symmetric) argument matrix data, for symmetric, is also use to output eigen vectors
 * @param iCols int nb of rows/cols of matrix
 * @param computeEigenVectors int (boolean semantics) only need for symmetric : indicates weither eigenvectors should be computed
 *
 * out :
 * eigen values either real or complex in 'z' format
 * @param pEigenValues double[complex]* output eigen values
 *
 *  or complex in 'c' format
 * @param pEigenValuesReal double* output eigen values real part
 * @param pEigenValuesImg double* output eigen values imaginary part
 *
 * for unsymmetric cases, output complex eigen vectors either in 'z' format (complex argument)
 * @param pEigenVectors doublecomplex* output eigen vectors
 *
 * or in 'c' format (real argument)
 * @param pEigenVectorsReal double* output eigen vectors real part
 * @param pEigenVectorsImg double* output eigen vectors imaginary part
 *
 */
extern int iEigen1ComplexSymmetricM(doublecomplex* pData, int iCols, int computeEigenVectors, double* pEigenValues);
extern int iEigen1RealSymmetricM(double* pData, int iCols, int computeEigenVectors, double* pEigenValues);

extern int iEigen1ComplexM(doublecomplex* pData, int iCols, doublecomplex* pEigenValues, doublecomplex* pEigenVectors);
extern int iEigen1RealM(double* pData, int iCols, double* pEigenValuesReal, double* pEigenValuesImg, double* pEigenVectorsReal, double* pEigenVectorsImg);

/*
 * With 2 lhs, eigenvalues are expanded from a 1D vector to the diagonal of a matrix.
 * The resulting matrix is a diagonal one: m[i,j]=0. for i!=j .
 * For efficiency reasons, we also convert from 'z' to 'c' format at the same time. In this case, conversion cannont be done in place.
 *
 * /!\ For the real case, expansion is done in place, so obviously the memory must have been reserved accordingly /!\
 * (i.e for the iCols x iCols matrix, not only the iCols x 1 vector.)
 *
 * @param pData double[complex]* : input  (&output for real) pData[0:iCols[ contains diagonal elements
 * @param iCols int : input nb of elements
 * 
 * out:
 * @param  pRMatrix double* real part of the diagonal output matrix
 * @param  pIMatrix double* imaginary part of the diagonal output matrix
 */
extern void expandToDiagonalOfMatrix(double* pData, int iCols);
extern void expandZToDiagonalOfCMatrix(doublecomplex const* pZVector, int iCols, double* pRMatrix, double* pIMatrix);



/*
 * Functions used to compute eigen values (and possibly eigen vectors) of two [complex|real]  matrix.
 *
 * @param pData1 double[complex]* in argument1 matrix data
 * @param pData2 double[complex]* in argument2 matrix data
 *
 * @param iCols int nb of rows/cols of matrix
 *
 * out : when something should not be computed (depending on lhs), output buffer ptr is NULL
 *
 * Alpha is either
 * in 'z' format for complex matrix
 * @param pAlpha doublecomplex*
 *
 * or 'c' format for real matrix
 *
 * @param pAlphaReal double*
 * @param pAlphaImg double*
 *
 *
 * @param pBeta double[complex]* 
 *
 * L and R are either
 * in 'z' format for complex matrix
 * @param pL doublecomplex*
 * @param pR doublecomplex*
 *
 * or 'c' format for real matrix
 *
 * @param pLReal double*
 * @param pLImg double*
 * @param pRReal double*
 * @param pRImg double*
 *
 */

extern int iEigen2ComplexM(doublecomplex* pData1, doublecomplex* pData2, int iCols
		     , doublecomplex* pAlpha, doublecomplex* pBeta, doublecomplex* pR, doublecomplex* pL);
extern int iEigen2RealM(double* pData1, double* pData2, int iCols
		     , double* pAlphaReal, double* pAlphaImg, double* pBeta, double* pRReal, double* pRimg, double* pLReal, double* pLImg);

#endif
