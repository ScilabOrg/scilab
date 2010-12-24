// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// 
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 8532 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8532
//
// <-- Short Description -->
// size of input arguments of read_csv was no verified.
//

A = 1:50;
mputl(strcat(string(A),ascii(9)+" "), TMPDIR + '/foo.csv');

if execstr("B = read_csv(TMPDIR + ""/foo.csv"", ["" "", ""\t""]);", "errcatch") <> 10000 then pause, end
msg = lasterror();
if msg <> msprintf(gettext("%s: Wrong size for input argument #%d: a string expected.\n"), "read_csv", 2) then pause, end

if execstr("B1 = read_csv(TMPDIR + ""/foo.csv"", ""\t"");", "errcatch") <> 0 then pause, end
if execstr("B2 = read_csv(TMPDIR + ""/foo.csv"", "" "");", "errcatch") <> 0 then pause, end
if or(evstr(B1) <> evstr(B2)) then pause, end
if or(A <> evstr(B2)) then pause, end