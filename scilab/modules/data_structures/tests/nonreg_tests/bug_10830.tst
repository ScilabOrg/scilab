// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 10830 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10830
//
// <-- Short Description -->
// Hypermatrix insertion with a negative (or null) index gives a wrong error message.

A=rand(2,2,2);
errmsg = msprintf(gettext("%s: Invalid index #%d in hypermatrix insertion.\n"),"hmops",3);
execstr("A(:,:,0)=[1,2;3,4]","errcatch");
assert_checktrue(lasterror()==errmsg);
errmsg = msprintf(gettext("%s: Invalid index #%d in hypermatrix insertion.\n"),"hmops",2);
execstr("A(:,0,1)=[1;2]", "errcatch");
assert_checktrue(lasterror()==errmsg);
errmsg = msprintf(gettext("%s: Invalid index #%d in hypermatrix insertion.\n"),"hmops",1);
execstr("A(0,:,1)=[1 2]", "errcatch");
assert_checktrue(lasterror()==errmsg);
