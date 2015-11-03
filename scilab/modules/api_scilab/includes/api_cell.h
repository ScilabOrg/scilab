/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enteprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#ifndef __CELL_API__
#define __CELL_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_cell.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createCell       API_PROTO(createCell)
#define scilab_createCell2d     API_PROTO(createCell2d)
#define scilab_getCellArray     API_PROTO(getCellArray)
#define scilab_setCellArray     API_PROTO(setCellArray)

    scilabVar API_PROTO(createCell)(scilabEnv env, int dim, const int* dims);
    scilabVar API_PROTO(createCell2d)(scilabEnv env, int row, int col);

    api_return API_PROTO(getCellArray)(scilabEnv env, scilabVar var, const scilabVar** vals);
    api_return API_PROTO(setCellArray)(scilabEnv env, scilabVar var, const scilabVar* vals);
#ifdef __cplusplus
}
#endif
#endif /* __CELL_API__ */
