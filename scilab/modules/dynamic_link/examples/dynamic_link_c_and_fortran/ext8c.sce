
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Demo file for ext8c example

// builder code for ext8c.c
link_name = "ext8c";    // functions to be added to the call table
flag  = "c";		// ext8c is a C function
files = ["ext8c.c" ];   // objects files for ext8c
libs  = [];		// other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext8c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call
// using the previously generated loader
exec loader.sce;

// test new function through the call function
//same example as # 10 with call to matptr
//param must be defined as a scilab variable

param=[1,2,3]
y=call("ext8c","out",size(param),1,"d");
if norm(y-param) > %eps then pause,end
if norm(param(1)-18.0) > %eps then pause,end

