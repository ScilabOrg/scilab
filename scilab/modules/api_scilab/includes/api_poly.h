/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#ifndef __POLY_API__
#define __POLY_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_poly.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createPoly       API_PROTO(createPoly)
#define scilab_createPoly2d     API_PROTO(createPoly2d)
#define scilab_getPoly          API_PROTO(getPoly)
#define scilab_getComplexPoly   API_PROTO(getComplexPoly)
#define scilab_setPoly          API_PROTO(setPoly)
#define scilab_setComplexPoly   API_PROTO(setComplexPoly)

scilabVar API_PROTO(createPoly)(scilabEnv env, wchar_t* varname, int dim, int* dims, int complex);
scilabVar API_PROTO(createPoly2d)(scilabEnv env, wchar_t* varname, int row, int col, int complex);

api_return API_PROTO(getPoly)(scilabEnv env, scilabVar var, int index, double** real);
api_return API_PROTO(getComplexPoly)(scilabEnv env, scilabVar var, int index, double** real, double** img);

api_return API_PROTO(setPoly)(scilabEnv env, scilabVar var, int index, int rank, double* real);
api_return API_PROTO(setComplexPoly)(scilabEnv env, scilabVar var, int index, int rank, double* real, double* img);

#ifdef __cplusplus
}
#endif
#endif /* __POLY_API__ */
