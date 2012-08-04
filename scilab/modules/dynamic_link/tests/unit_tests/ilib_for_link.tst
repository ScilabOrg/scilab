// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

if getos() == 'Windows' then
  TMP_OS_DIR = getenv('TMP','err');
  if (TMP_OS_DIR == 'err') then pause,end;
else
  TMP_OS_DIR = '/tmp';
  if ~isdir(TMP_OS_DIR) then pause,end;
end

TMP_DIR = TMP_OS_DIR+filesep()+'ilib_for_link';

rmdir(TMP_DIR,'s');
mkdir(TMP_OS_DIR,'ilib_for_link');


f1=['int ext1c(int *n, double *a, double *b, double *c)'
    '{int k;'
    '  for (k = 0; k < *n; ++k) '
    '      c[k] = a[k] + b[k];'
    '  return(0);}'];

mputl(f1,TMP_DIR+filesep()+'fun1.c');

//creating the shared library (a gateway, a Makefile and a loader are 
//generated. 

cur_dir = pwd();

chdir(TMP_DIR);

ilib_for_link('ext1c','fun1.c',[],"c") 

// load the shared library 
exec loader.sce;

chdir(cur_dir);

//using the new primitive
a=[1,2,3];b=[4,5,6];n=3;
c=call('ext1c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(a+b)) > %eps then pause,end

// ulink() all libraries
ulink();

//remove TMP_DIR
//rmdir(TMP_DIR,'s');