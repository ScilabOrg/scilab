// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- JVM NOT MANDATORY -->
// =============================================================================
// <-- Non-regression test for bug 245 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/245/
//
// <-- Short Description -->
// csv_stringtodouble failed on some special cases.

path = SCI+"/modules/spreadsheet/tests/unit_tests/";

a = csv_stringtodouble("3.14159e-10+inf");
assert_checkequal ( isnan(a) , %t );

assert_checkequal ( execstr("a = csv_stringtodouble(""3.14159e-10+inf"", %f);", "errcatch") , 999 );
assert_checkequal ( lasterror() , msprintf(_("%s: can not convert data.\n"), "csv_stringtodouble") );

a = csv_stringtodouble("6+3*I");
assert_checkequal ( isnan(a) , %t );

assert_checkequal ( execstr("a = csv_stringtodouble(""6+3*I"", %f);", "errcatch") , 999 );
assert_checkequal ( lasterror() , msprintf(_("%s: can not convert data.\n"), "csv_stringtodouble") );
// =============================================================================
