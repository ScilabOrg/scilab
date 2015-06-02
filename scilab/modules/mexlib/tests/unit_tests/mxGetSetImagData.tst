// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// ============================================================================
// Unitary tests for mxGetImagData and mxSetImagData mex functions
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    void *data = mxGetImagData(prhs[0]);";
"    mxSetImagData(prhs[0], data);";
"    plhs[0] = prhs[0];";
"}"],"mexGetSetImagData.c");
ilib_mex_build("libmextest",["getSetImagData","mexGetSetImagData","cmex"], "mexGetSetImagData.c",[]);
exec("loader.sce");

r = getSetImagData(complex(1, 2));
assert_checkequal(r(1), complex(1, 2));
