//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1659 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1659
//
// <-- Short Description -->
//'errclear' does not clear last error message/number

function y=f1(a, b);y=a+b;endfunction

function m=f2()
    errclear();
    m=lasterror();
endfunction

clear b
err=execstr("f1(1)","errcatch");
assert_checkfalse(err == 0);
assert_checkequal(f2(), []);

