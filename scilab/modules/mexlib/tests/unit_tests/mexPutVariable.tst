// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// ============================================================================
// Unitary tests for mexPutVariable mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    char *workspace = mxArrayToString(prhs[0]);";
"    char *name = mxArrayToString(prhs[1]);";
"    mxArray *pOut = mexPutVariable(workspace, name, prhs[2]);";
"}"],"mexputVariable.c");
ilib_mex_build("libmextest", ["putVariable", "mexputVariable", "cmex"], "mexputVariable.c", [], "", "", "", "");
exec("loader.sce");

global globalvar;
putVariable("global", "globalvar", 32);
assert_checkequal(globalvar, 32);
assert_checktrue(isglobal("globalvar"));

othervar = 1;
putVariable("base", "othervar", 3);
assert_checkequal(othervar, 1);
assert_checkfalse(isglobal("othervar"));

putVariable("caller", "othervar", 3);
assert_checkequal(othervar, 3);
assert_checkfalse(isglobal("othervar"));

putVariable("caller", "caller_defined_in_C", 0);
assert_checktrue(exists("caller_defined_in_C")==1);
