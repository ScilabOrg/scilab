// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
//
// ============================================================================
// Unitary tests for mxGetFieldNumber mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    int num = mxGetFieldNumber(prhs[0], ""a"");";
"    if(num == -1)"
"    {"
"        plhs[0] = mxCreateDoubleScalar(num);";
"    }"
"    else"
"    {"
"        plhs[0] = mxCreateDoubleScalar(num + 1);";
"    }"
"}"],"mexgetFieldNumber.c");
ilib_mex_build("libmextest",["getFieldNumber","mexgetFieldNumber","cmex"], "mexgetFieldNumber.c",[]);
exec("loader.sce");

c = struct("a", 3, "b", 1, "1", 2);
assert_checkequal(getFieldNumber(c), 1);

c = struct("b", 3, "a", 5);
assert_checkequal(getFieldNumber(c), 2);

c = struct("b", 32);
assert_checkequal(getFieldNumber(c), -1);

assert_checkequal(getFieldNumber("not_a_struct"), -1);
