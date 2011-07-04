// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxCreateDoubleMatrix, mxCreateDoubleScalar mex function
// ============================================================================

cd(TMPDIR);
mputl([ '#include ""mex.h""';
        '';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '   int i = 0;';
        '   double dblM = 0;';
        '   double dblN = 0;';
        '   double *pdbl = NULL;';
        '   mxArray* pOut1 = NULL;';
        '   mxArray* pOut2 = NULL;';
        '';
        '   if(nrhs != 2 || nlhs != 2 /*|| !mxIsDouble(prhs[0])*/)';
        '   {';
        '       mexErrMsgTxt(""Wrong number or type of input argument"");';
        '   }';
        '';
        '   dblM = mxGetScalar(prhs[0]);';
        '   dblN = mxGetScalar(prhs[1]);';
        '   pOut1 = mxCreateDoubleMatrix((int)dblM, (int)dblN, mxREAL);';
        '   pOut2 = mxCreateDoubleScalar(dblM * dblN);';
        '   pdbl = mxGetPr(pOut1);';
        '   for(i = 0 ; i < dblM * dblN ; i++)';
        '   {';
        '       pdbl[i] = i+1;';
        '   }';
        '';
        '   plhs[0] = pOut1;';
        '   plhs[1] = pOut2;';
        '}'],'mexCreateDoubleScalar.c');
ilib_mex_build('libmex',['createDouble','mexCreateDoubleScalar','cmex'],'mexCreateDoubleScalar.c',[],'Makelib','','','');
exec('loader.sce');

m = 3;
n = 4;
[a,b] = createDouble(m, n);
if b <> m*n then pause end
if or(a(:)' <>  (1:m*n)) then pause end
