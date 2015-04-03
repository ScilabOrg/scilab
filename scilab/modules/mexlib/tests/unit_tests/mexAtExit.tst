// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- NOT FIXED -->
// ============================================================================
// Unitary tests for mexAtExit mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void callAtExit()";
"{";
"    mexPrintf(""The mexAtExit function works!"");";
"}";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    mexAtExit(&callAtExit);";
"}"],"mexatExit.c");
ilib_mex_build("libmextest",["atExit","mexatExit","cmex"], "mexatExit.c",[]);
exec("loader.sce");

atExit()
