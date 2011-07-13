// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetFieldNumber mex function
// ============================================================================

cd(TMPDIR);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    int num = mxGetFieldNumber(prhs[0], ""a"");';
       '    plhs[0] = mxCreateDoubleScalar(num);';
       '}'],'mexgetFieldNumber.c');
ilib_mex_build('libmextest',['getFieldNumber','mexgetFieldNumber','cmex'], 'mexgetFieldNumber.c',[],'Makelib','','','');
exec('loader.sce');

c = struct("a", 3, "b", 1, "1", 2);
r = getFieldNumber(c);
if r <> 1 then pause end

c = struct("a", 5, "b", 3);
r = getFieldNumber(c);
if r <> 0 then pause end

c = struct("b", 32);
r = getFieldNumber(c);
if r <> -1 then pause end

r = getFieldNumber("not_a_struct");
if r <> -1 then pause end
