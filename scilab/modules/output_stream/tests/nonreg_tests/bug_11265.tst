// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 11265 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11265
//
// <-- Short Description -->
// msprintf crashed on a huge string
//

r = rand(35, 1024); // 35k values
str = string(r);
s = strcat(str, " , ");
msgerr = msprintf(gettext("%s: An error occurred: %s\n"), "msprintf", gettext("Buffer too small."));
assert_checkerror ("tts = sprintf(''%s'', s);", msgerr);

n = 32;
r = rand(n, 128);
str = string(r);
s = strcat(str, " , ");
msgerr = msprintf(gettext("%s: An error occurred: %s\n"), "msprintf", gettext("Buffer too small."));
assert_checkerror ("tts = msprintf(''%s'', s)", msgerr);
