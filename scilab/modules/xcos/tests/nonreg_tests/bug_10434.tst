// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 10434 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10434
//
// <-- Short Description -->
// 
//

write(TMPDIR+'/bug_10434.predef', predef());
prot=funcprot();
funcprot(0);
loadXcosLibs();
funcprot(prot);

clear();

assert_checktrue(isdef("xcos_simulate"));

previous_predef=read(TMPDIR+'/bug_10434.predef', 1, 1);
deletefile(TMPDIR+'/bug_10434.predef');
assert_checkequal(predef(), previous_predef);
