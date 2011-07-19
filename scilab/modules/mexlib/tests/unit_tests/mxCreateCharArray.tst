// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxCreateCharArray mex function
// ============================================================================

cd(TMPDIR);

mputl(['#include ""mex.h""';
       '#include <wchar.h>';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    int dims[2] = {1, 1};';
       '    wchar_t *data[1];';
       '    data[0] = L""hello world"";';
       '    mxArray *pOut = mxCreateCharArray(2, dims);';
       '    // test breaks without data';
       '    mxSetData(pOut, data);';
       '    plhs[0] = pOut;';
       '}'],'mexcreateCharArray.c');
ilib_mex_build('libmextest',['createCharArray','mexcreateCharArray','cmex'], 'mexcreateCharArray.c',[],'Makelib','','','');
exec('loader.sce');

r = createCharArray();
if r(1) <> "hello world" then pause end
