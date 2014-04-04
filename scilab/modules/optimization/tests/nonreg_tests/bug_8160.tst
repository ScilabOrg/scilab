// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 8160 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8160
//
// <-- Short Description -->
// The derivative function did not check for input argument #1, if it is a list.
//



x=[1;2;3];

assert_checkfalse(execstr("derivative(list(),x)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of elements in input argument #%d: At least %d elements expected, but current number is %d.\n"), "derivative", 1, 2, 0);
assert_checkerror("derivative(list(),x)", refMsg);

assert_checkfalse(execstr("derivative(list(""foo""),x)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of elements in input argument #%d: At least %d elements expected, but current number is %d.\n"), "derivative", 1, 2, 1);
assert_checkerror("derivative(list(""foo""),x)", refMsg);


assert_checkfalse(execstr("derivative(list(""foo"",""aa""),x)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for element #%d in input argument #%d: A function is expected, but current type is %s.\n"), "derivative", 1, 1, typeof("a"));
assert_checkerror("derivative(list(""foo"",""aa""),x)", refMsg);





