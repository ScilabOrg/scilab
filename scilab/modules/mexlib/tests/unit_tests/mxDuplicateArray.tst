// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxDuplicateArray mex function
// ============================================================================

cd(TMPDIR);
mputl([ '#include ""mex.h""';
        '';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '   mxArray* pOut = NULL;';
        '';
        '   if(nrhs != 1 /*|| !mxIsDouble(prhs[0])*/)';
        '   {';
        '       mexErrMsgTxt(""Wrong number or type of input argument"");';
        '   }';
        '';
        '   pOut = mxDuplicateArray(prhs[0]);';
        '   plhs[0] = pOut;';
        '}'],'mexDuplicateArray.c');
ilib_mex_build('libmextest',['duplicateArray','mexDuplicateArray','cmex'], 'mexDuplicateArray.c',[],'Makelib','','','');
exec('loader.sce');

a = rand(4,3);
b = duplicateArray(a);
if a <> b then pause end
