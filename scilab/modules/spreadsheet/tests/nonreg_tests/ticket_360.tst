// =============================================================================
// Copyright (C) 2011 - INRIA - Michael Baudin
// =============================================================================
// <-- JVM NOT MANDATORY -->
// =============================================================================
// <-- Non-regression test for bug 360 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/360/
//
// <-- Short Description -->
// The csvRead function does not manage the range.
// =============================================================================
path = SCI+"/modules/spreadsheet/tests/unit_tests/";

//
// Read complex doubles with csvTextScan
Astr = mgetl(fullfile(path,"complexdata.csv"));
r = csvTextScan(Astr,[],[],"double");
expected = [
%nan
%nan
];
assert_checkequal ( r , expected );

//
// Read complex doubles with csvRead
r = csvRead(fullfile(path,"complexdata.csv"), [], [], "double" );
expected = [
%nan
%nan
];
assert_checkequal ( r , expected );
//
// Configure the separator
r = csvTextScan(Astr," ",[],"double");
expected = [6+3*%i 13-7*%i 20+4*%i 27-1.5*%i 34+3.14*%i 41-3*%i 48+3*%i 55-7*%i
                +0      -0    %inf     -%inf       %nan 1.D+308 1.e-308 1.e-323];
assert_checkalmostequal( r , expected );
// =============================================================================
