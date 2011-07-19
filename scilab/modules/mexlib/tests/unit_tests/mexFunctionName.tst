// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mexFunctionName mex function
// ============================================================================

cd(TMPDIR);

mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    mexPrintf(""%s"", mexFunctionName());';
       '}'],'mexfunctionName.c');

ilib_mex_build('libmex',['func','mexfunctionName','cmex'], 'mexfunctionName.c',[],'Makelib','','','');
exec('loader.sce');

func();
