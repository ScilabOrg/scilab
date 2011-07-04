// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxCreateLogicalScalar mex function
// ============================================================================

cd(TMPDIR);
mputl([ '#include ""mex.h""';
        '';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '   int bIn1 = 0;';
        '   int bIn2 = 0;';
        '   int bOut1 = 0;';
        '   int bOut2 = 0;';
        '   mxArray* pOut1 = NULL;';
        '   mxArray* pOut2 = NULL;';
        '';
        '   if(nrhs != 2 || nlhs != 2/*|| !mxIsDouble(prhs[0])*/)';
        '   {';
        '       mexErrMsgTxt(""Wrong number or type of input argument"");';
        '   }';
        '';
        '   bIn1 = mxIsLogicalScalarTrue(prhs[0]);';
        '   bIn2 = mxIsLogicalScalarTrue(prhs[1]);';
        '';
        '   bOut1 = !bIn1;';
        '   pOut1 = mxCreateLogicalScalar(bOut1);';
        '';
        '   bOut2 = !bIn2;';
        '   pOut2 = mxCreateLogicalScalar(bOut2);';
        '';
        '   plhs[0] = pOut1;';
        '   plhs[1] = pOut2;';
        '}'],'mexCreateLogicalScalar.c');
ilib_mex_build('libmex',['createLogicalScalar','mexCreateLogicalScalar','cmex'],[],[],'Makelib','','','');
exec('loader.sce');

[out1, out2] = createLogicalScalar(%t, %f);

if out1 <> %f then pause end
if out2 <> %t then pause end

