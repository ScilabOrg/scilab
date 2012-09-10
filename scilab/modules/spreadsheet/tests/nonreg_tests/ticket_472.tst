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
// csv_read, csv_textscan cannot ignore blank lines.
// =============================================================================
txt = ["my data 1";"";"my data 2"];
ref_res = ["my data 1";"my data 2"];

assert_checkequal(csv_default('blank'), 'off');
ierr = execstr("res = csv_textscan(txt, [] , [], ""string"")", "errcatch");
assert_checkequal(ierr, 999);

assert_checkequal(csv_default('blank','on'), %t);
ierr = execstr("res = csv_textscan(txt, [] , [], ""string"")", "errcatch");
assert_checkequal(ierr, 0);
assert_checkequal(res, ref_res);
// =============================================================================
