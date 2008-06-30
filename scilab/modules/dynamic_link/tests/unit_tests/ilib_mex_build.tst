// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

cd(TMPDIR);

mputl([
'#include ""mex.h""'
'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])'
'{'
'  int *dims = mxGetDimensions(prhs[0]);'
'  sciprint(""%d %d %d\n"",dims[0],dims[1],dims[2]);'
'}'
],'mexfunction16.c');

if ~MSDOS then
  ilib_mex_build('libmex',['mexf16','mexFunction','cmex'],['mexfunction16.c'],[],'Makelib','','','');
else
  ilib_mex_build('libmex',['mexf16','mexfunction16','cmex'],[],[],'Makelib','','','');
end
exec(TMPDIR+'/loader.sce');
mexf16(rand(2,3,2));


