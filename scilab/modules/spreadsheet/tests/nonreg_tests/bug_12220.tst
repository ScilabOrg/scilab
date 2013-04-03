// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// 
// <-- JAVA NOT MANDATORY -->
//
// <-- Non-regression test for bug 12220 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12220
//
// <-- Short Description -->
// csvTextScan unexpected behavior for missing data
//

content_ref = [ "26,12-10-2011 18:00,29.83,69.1,42,35.5,83,30.8,35.5,0,SSW,0,0,0" ..
"27,12-10-2011 19:00,29.83,68.9,42,35.6,84,31.3,35.6,0,SSW,0,0,0" ..
"28,12-10-2011 20:00,29.83,68.2,43,35.6,84,31.3,,,,,0,0" ..
"29,12-10-2011 21:00,29.82,67.3,43,35.3,84,31,,,,,0,0" ];
filename=TMPDIR+"/foo.csv";

mputl(content_ref,filename);

StrLines = mgetl(filename);
StrMat1 = csvTextScan(StrLines(1), ',', '.', 'string');
StrMat3 = csvTextScan(StrLines(3), ',', '.', 'string');

assert_checkequal(size(StrMat1), size(StrMat3));
