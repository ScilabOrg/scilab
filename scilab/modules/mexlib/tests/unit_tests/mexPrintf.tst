// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// ============================================================================
// Unitary tests for mexIsGlobal mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    mexPrintf(""The quick brown fox jumps over the lazy dog.\n"");";
"    mexPrintf(""Some UTF-8 chars are áãâéíóúü.\n"");";
"    mexPrintf(""Printing %s %s."", ""with"", ""parameters"");";
"}"],"mexprintf.c");
ilib_mex_build("libmextest", ["mprintf", "mexprintf", "cmex"], "mexprintf.c", [], "", "", "", "");
exec("loader.sce");

mprintf();
