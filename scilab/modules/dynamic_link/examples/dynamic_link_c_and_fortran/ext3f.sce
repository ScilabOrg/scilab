
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Demo file for ext3f example

// builder code for ext3f.c
link_name = "ext3f";    // functions to be added to the call table
flag  = "f";		// ext3f is a Fortran function
files = ["ext3f.f" ];   // objects files for ext3f
libs  = [];		// other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext3f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call
// using the previously generated loader
exec loader.sce;

// test new function through the call function
//reading  vector with name='a' in scilab internal stack

a=[1,2,3];b=[4,5,6];n=3;
c=call("ext3f","yes",1,"c",n,2,"i",a,3,"d",b,4,"d","out",[1,3],5,"d");
if norm(c-(sin(a)+cos(b)))> %eps then pause,end
c=call("ext3f","no",1,"c",n,2,"i",a,3,"d",b,4,"d","out",[1,3],5,"d");
if norm(c-(a+b)) > %eps then pause,end

