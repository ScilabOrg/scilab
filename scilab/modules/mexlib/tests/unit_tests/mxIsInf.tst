// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxIsInf mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
ieee(2);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    double value = mxGetScalar(prhs[0]);";
"    bool isInf = mxIsInf(value);";
"    mxArray* pOut = mxCreateLogicalScalar(isInf);";
"    plhs[0] = pOut;";
"}"],"mexisInf.c");
ilib_mex_build("libmextest",["isInf","mexisInf","cmex"], "mexisInf.c",[]);
exec("loader.sce");

assert_checktrue(isInf(%inf));
assert_checktrue(isInf(1/0));
assert_checkfalse(isInf(1));
assert_checkfalse(isInf(%nan));
