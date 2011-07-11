// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxIsLogicalScalar mex function
// ============================================================================

cd(TMPDIR);
mputl([ '#include ""mex.h""';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '    plhs[0] = mxCreateLogicalScalar(mxIsLogicalScalar(prhs[0]));';
        '    plhs[1] = mxCreateLogicalScalar(mxIsLogicalScalarTrue(prhs[0]));';
        '}'],'mexisLogicalScalar.c');
ilib_mex_build('libmex', ['isLogicalScalar', 'mexisLogicalScalar', 'cmex'], 'mexisLogicalScalar.c', [], 'Makelib', '', '', '');
exec('loader.sce');

[out1, out2] = isLogicalScalar(%t);
[out3, out4] = isLogicalScalar(%f);

if out1 <> %t then pause end
if out2 <> %t then pause end
if out3 <> %t then pause end
if out4 <> %f then pause end
