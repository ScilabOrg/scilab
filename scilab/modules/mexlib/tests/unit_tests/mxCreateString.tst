// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxCreateString mex function
// ============================================================================

cd(TMPDIR);
mputl([ '#include ""mex.h""';
        '';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '   mxArray* pOut = NULL;';
        '';
        '   if(nrhs != 0 /*|| !mxIsDouble(prhs[0])*/)';
        '   {';
        '       mexErrMsgTxt(""Wrong number or type of input argument"");';
        '   }';
        '';
        '   pOut = mxCreateString(""Create new String"");';
        '   plhs[0] = pOut;';
        '}'],'mexCreateString.c');
ilib_mex_build('libmex',['createString','mexCreateString','cmex'], 'mexCreateString.c',[],'Makelib','','','');
exec('loader.sce');

ref = "Create new String";
a = createString();
if a <> ref then pause end
