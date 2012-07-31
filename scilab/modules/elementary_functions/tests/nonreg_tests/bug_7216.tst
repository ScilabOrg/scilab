// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7216 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7216
//
// <-- Short Description -->
//    modulo(x,0) and pmodulo(x,0) return an error message. With ieee(2), return 
//    %nan
// =============================================================================

assert_checkfalse(execstr("modulo(5,0)", "errcatch") == 0);
refMsg = msprintf(_("Division by zero...\n")); 
assert_checkerror("modulo(5,0)", refMsg);

assert_checkfalse(execstr("pmodulo(5,0)", "errcatch") == 0);
refMsg = msprintf(_("Division by zero...\n")); 
assert_checkerror("pmodulo(5,0)", refMsg);

ieee(2)
x = modulo(5,0);
assert_checkequal(x, %nan);

x = pmodulo(5,0);
assert_checkequal(x, %nan);

ieee(0)


