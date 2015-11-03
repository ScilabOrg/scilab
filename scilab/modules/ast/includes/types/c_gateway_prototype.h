/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __STACK_GATEWAY_PROTOTYPE_H__
#define __STACK_GATEWAY_PROTOTYPE_H__

typedef void* scilabEnv;
typedef int* scilabVar;
#define STACK_GATEWAY_PROTOTYPE(__gateway__) int __gateway__(char* fname, void* pvApiCtx)
#define C_GATEWAY_PROTOTYPE(__gateway__) int __gateway__(void* env, int nin, int** in, int nout, int** out)
typedef int(*OLDGW_FUNC)(char *fname, void* pvApiCtx);
typedef int(*GW_C_FUNC)(scilabEnv env, int nin, scilabVar* in, int nout, scilabVar* out);
typedef int(*MEXGW_FUNC)(int nlhs, int* plhs[], int nrhs, int* prhs[]);
#define MEX_GATEWAY_PROTOTYPE(__gateway__) int (__gateway__) (int nlhs, int* plhs[], int nrhs, int* prhs[]);

#endif /* !__STACK_GATEWAY_PROTOTYPE_H__ */
