// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// <-- INTERACTIVE TEST -->
// ============================================================================
// Unitary tests for mxAssert mex function
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    mxAssert(mxGetScalar(prhs[0]), mxGetChars(prhs[1]));';
       '}'],'mexAssert.c');
<<<<<<< HEAD
ilib_mex_build('libmextest',['assert','mexAssert','cmex'], 'mexAssert.c',[],'Makelib','','-D NDEBUG','');
=======
ilib_mex_build('libmextest',['assert','mexAssert','cmex'], 'mexAssert.c',[],'Makelib','','','');
>>>>>>> Some mexlib unit tests.
exec('loader.sce');

assert(1, 'This is true');
assert(0, 'This is false and throws an exception.');
assert(1, 'This assert is never executed');
