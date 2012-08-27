// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//exec SCI/modules\api_scilab\tests\unit_tests\deleteNamedVariable.tst
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/deleteNamedVariable"));
cd(pathconvert(TMPDIR+"/deleteNamedVariable"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/deleteNamedVariable.c",pathconvert(TMPDIR+"/deleteNamedVariable/deleteNamedVariable.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
setenv('DEBUG_SCILAB_DYNAMIC_LINK','YES');
ilib_build("gw_deleteNamedVariable", ["deleteNamedVariable", "sci_deleteNamedVariable"], "deleteNamedVariable.c", [], "", "", cflags);
exec("loader.sce");

function test()
    assert_checkequal(exists("a"), 1);
    assert_checkequal(a, 1);
    a = 2;
    assert_checkequal(a, 2);
    assert_checktrue(deleteNamedVariable("a"));
    assert_checkequal(exists("a"), 1);
    assert_checkequal(a, 1);
endfunction

a = 1;
test();
assert_checkequal(a, 1);
deleteNamedVariable("a");
assert_checkequal(exists("a"), 0);
