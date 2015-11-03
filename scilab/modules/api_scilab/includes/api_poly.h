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

#define scilab_createPolyMatrix     API_PROTO(createPolyMatrix)
#define scilab_createPolyMatric2d   API_PROTO(createPolyMatrix2d)
#define scilab_getPolyArray         API_PROTO(getPolyArray)
#define scilab_getComplexPolyArray  API_PROTO(getComplexPolyArray)
#define scilab_setPolyArray         API_PROTO(setPolyArray)
#define scilab_setComplexPolyArray  API_PROTO(setComplexPolyArray)

    scilabVar API_PROTO(createPolyMatrix)(scilabEnv env, wchar_t* varname, int dim, int* dims, int complex);
    scilabVar API_PROTO(createPolyMatrix2d)(scilabEnv env, wchar_t* varname, int row, int col, int complex);

    int API_PROTO(getPolyArray)(scilabEnv env, scilabVar var, int index, double** real);
    int API_PROTO(getComplexPolyArray)(scilabEnv env, scilabVar var, int index, double** real, double** img);

    api_return API_PROTO(setPolyArray)(scilabEnv env, scilabVar var, int index, int rank, double* real);
    api_return API_PROTO(setComplexPolyArray)(scilabEnv env, scilabVar var, int index, int rank, double* real, double* img);

#ifdef __cplusplus
}
#endif
#endif /* __POLY_API__ */
