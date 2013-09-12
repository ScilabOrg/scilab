
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Demo file for ext12c example

// builder code for ext12c.c
link_name = "ext12c";    // functions to be added to the call table
flag  = "c";		 // ext12c is a C function
files = ["ext12c.c" ];   // objects files for ext12c
libs  = [];		 // other libs needed for linking

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext12c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call
// using the previously generated loader
exec loader.sce;

// test new function through the ode function
//same example as # 10 with call to matptr
//param must be defined as a scilab variable

param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],"ext12c");


