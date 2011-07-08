// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetString mex function
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    char string[20];';
       '    mxGetString(prhs[0], string, 20);';
       '    mexPrintf(""%s"", string);';
       '}'],'mexgetString.c');
ilib_mex_build('libmextest',['getString','mexgetString','cmex'], 'mexgetString.c',[],'Makelib','','','');
exec('loader.sce');

getString('String to copy.');
