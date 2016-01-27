//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===
//
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//

cd TMPDIR;

//create 2 lib with the same symbol ( "lib_test" for exemple )

//create the "lib_test" macro file in TMPDIR/lib1
mkdir("lib1");
mputl(["function ret = lib_test";"  ret= ""lib1""";"endfunction"], "lib1/lib_test.sci");
//generate lib ( aka "lib1lib" )
genlib("lib1lib","lib1",%f,%t);
assert_checkequal(lib_test(), "lib1");
clear lib1lib lib_test;

//same things with TMPDIR/lib2
mkdir("lib2");
mputl(["function ret = lib_test";"  ret= ""lib2""";"endfunction"], "lib2/lib_test.sci");
//generate lib ( aka "lib2lib" )
genlib("lib2lib","lib2",%f,%t);
assert_checkequal(lib_test(), "lib2");
clear lib2lib lib_test;

//now try to load lib1lib
lib1lib = lib("lib1");
//call macro
assert_checkequal(lib_test(), "lib1");
clear lib1lib lib_test;

//now try to load lib2lib
lib2lib = lib("lib2");
//call macro
assert_checkequal(lib_test(), "lib2");
clear lib2lib lib_test

//load lib1 call function, load lib2 and call function again
lib1lib = lib("lib1");
assert_checkequal(lib1lib.lib_test(), "lib1");
lib2lib = lib("lib2");
assert_checkequal(lib2lib.lib_test(), "lib2");


m = mgetl("SCI/modules/ast/tests/unit_tests/test_macro.sci");
mkdir("test");
mputl(m, "test/test_macro.sci");
genlib("testlib","test",%f,%t);
assert_checkequal(test_macro(4), 16);
assert_checkequal(exists("internal_macro"), 0);
assert_checkequal(exists("x"), 0);

internal_macro = 1;x = 18;
assert_checkequal(test_macro(5), 20);
assert_checkequal(internal_macro, 1);
assert_checkequal(x, 18);

// resume library
mkdir("mylib");
mputl("function mylib(), endfunction", "mylib/mylib.sci");
genlib("myliblib", "mylib",%f,%t);
function resumeLib()
    l=lib("mylib");
    l=resume(l);
endfunction

resumeLib();
resumeLib();
