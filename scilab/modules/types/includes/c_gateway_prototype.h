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
#ifndef __C_GATEWAY_PROTOTYPE_H__
#define __C_GATEWAY_PROTOTYPE_H__

#define C_GATEWAY_PROTOTYPE(__gateway__) int __gateway__(char* fname, void* pvApiCtx)
#define F_GATEWAY_PROTOTYPE(__gateway__) int __gateway__(char* fname)
typedef int (*OLDGW_FUNC)(char *fname, void* pvApiCtx);
typedef int (*OLDGWF_FUNC)(char *fname);
typedef int (*MEXGW_FUNC)(int nlhs, int* plhs[], int nrhs, int* prhs[]);

#endif /* !__C_GATEWAY_PROTOTYPE_H__ */
