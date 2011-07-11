// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxIsNaN mex function
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    double value = mxGetScalar(prhs[0]);';
       '    bool isNaN = mxIsNaN(value);';
       '    mxArray* pOut = mxCreateLogicalScalar(isNaN);';
       '    plhs[0] = pOut;';
       '}'],'mexisNaN.c');
ilib_mex_build('libmex',['isNaN','mexisNaN','cmex'], 'mexisNaN.c',[],'Makelib','','','');
exec('loader.sce');

a = isNaN(%inf);
if a <> %f then pause end
a = isNaN(1/0);
if a <> %f then pause end
a = isNaN(1);
if a <> %f then pause end
a = isNaN(%nan);
if a <> %t then pause end
