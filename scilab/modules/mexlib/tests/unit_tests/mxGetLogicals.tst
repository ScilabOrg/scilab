// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetLogicals mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    mxLogical *logicals = mxGetLogicals(prhs[0]);";
"    plhs[0] = mxCreateLogicalScalar(logicals[0]);";
"    plhs[1] = mxCreateLogicalScalar(logicals[1]);";
"}"],"mexgetLogicals.c");
ilib_mex_build("libmextest",["getLogicals","mexgetLogicals","cmex"], "mexgetLogicals.c",[]);
exec("loader.sce");

[out1, out2] = getLogicals([%t %f]);
assert_checktrue(out1);
assert_checkfalse(out2);
