
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Linking a .mexglx, say foo.mexglx, file with Scilab.
// (Assuming foo.mexglx has been created by the Matlab's mex script).
// 0/ Here I create the file foo.mexglx using Scilab, just to have a
// proper foo.mexglx for testing

ilib_for_link("foo",["foo.o"],[],"c");
host("cp libfoo.so foo.mexglx");

// 1/ If necessary, create empty libmx.so libmex.so and libmat.so which
//    could be required by the .mexglx file.
//    (If "ldd foo.mexglx" shows a dependency).
// This is done by the following commands:

ilib_for_link("mx",[],[],"c");
ilib_for_link("mex",[],[],"c");
ilib_for_link("mat",[],[],"c");

// 4/ Make a dynamic library with the provided C routine (libtst.c file).
//    Note that you can use libtst.c file as is and that the entrypoint
//    MUST BE mexFunction. If you have more than one mexglx files you
//    will need to copy libtst.c and change only the function name
//    (this is described below)

//  just make a lib*.so from ./foo.mexglx or else libtool will ignore ./foo.mexglx

host("cp ./foo.mexglx libfoo_mex.so")
ilib_for_link("tst",["libtst.o"]," -R`pwd` -L`pwd` -lfoo_mex -lmx -lmex -lmat","c");

//5/At Scilab prompt enter:

addinter("./libtst.so","libtst","foo");

// 6/call the mexfunction:

foo(5,"test string")
