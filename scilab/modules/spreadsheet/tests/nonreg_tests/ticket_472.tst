// =============================================================================
// Copyright (C) 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- JVM NOT MANDATORY -->
// =============================================================================
// <-- Non-regression test for bug 472 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/472/
//
// <-- Short Description -->
// csvRead, csvTextScan cannot ignore blank lines.
// =============================================================================

warning("off");
txt = ["my data 1";"";"my data 2"];
ref_res = ["my data 1";"my data 2"];

assert_checkequal(csvDefault('blank'), 'on');
assert_checkequal(csvDefault('blank','off'), %t);
ierr = execstr("res = csvTextScan(txt, [] , [], ""string"")", "errcatch");
assert_checkequal(ierr, 999);

assert_checkequal(csvDefault('blank','on'), %t);
ierr = execstr("res = csvTextScan(txt, [] , [], ""string"")", "errcatch");
assert_checkequal(ierr, 0);
assert_checkequal(res, ref_res);
// =============================================================================

assert_checkequal(csvDefault('blank','off'), %t);