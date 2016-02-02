
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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


