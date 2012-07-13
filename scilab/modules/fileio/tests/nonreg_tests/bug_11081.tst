// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 11081 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11081
//
// <-- Short Description -->
// movefile, copyfile did not support '~', 'TMPDIR', ... shortcuts in filenames
//

mputl('bug_11081','TMPDIR/bug_11081_txt_1.txt');
copyfile('TMPDIR/bug_11081_txt_1.txt', 'TMPDIR/bug_11081_txt_2.txt');
assert_checktrue(isfile('TMPDIR/bug_11081_txt_2.txt'));
movefile('TMPDIR/bug_11081_txt_2.txt', 'TMPDIR/bug_11081_txt_3.txt');
assert_checktrue(isfile('TMPDIR/bug_11081_txt_3.txt'));
