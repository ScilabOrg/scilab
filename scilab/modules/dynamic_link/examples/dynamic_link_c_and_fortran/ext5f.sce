
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Demo file for ext5f example

// builder code for ext5f.c
link_name = "ext5f";    // functions to be added to the call table
flag  = "f";  		// ext5f is a Fortran function
files = ["ext5f.f" ];   // objects files for ext5f
libs  = [];		// other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext5f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call
// using the previously generated loader
exec loader.sce;

// test new function through the call function
// reading vector a in scilab internal stack

a=[1,2,3];b=[2,3,4];
c=call("ext5f",b,1,"d","out",[1,3],2,"d");
if norm(c-(a+2*b)) > %eps then pause,end
