// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// ============================================================================
// Unitary tests for mxAddField mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    mxArray *ptr = prhs[0];"
"    int fieldnumber = mxAddField(ptr, ""another"");";
"    mxSetFieldByNumber(ptr, 0, fieldnumber, mxCreateDoubleScalar(3));";
"}"],"mexaddField.c");
ilib_mex_build("libmextest", ["addField", "mexaddField", "cmex"], "mexaddField.c", [], "", "", "", "");
exec("loader.sce");

s = struct();
addField(s);
assert_checkequal(s("another"), 3);
