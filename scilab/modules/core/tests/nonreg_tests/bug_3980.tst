// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3980 -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3980
//
// <-- Short Description -->
// Wrong line number in error message



deff("l=foo()","[l,m]=where()","n")
assert_checkequal(foo(), 2);

deff("foo","a=aaaa","n")
if execstr("foo()","errcatch") == 0 then pause,end
[str,n,l,f]=lasterror(%t);
assert_checkequal(l, 2);
assert_checkequal(f, "foo");

deff("foo","a=aaaa")
if execstr("foo()","errcatch") == 0 then pause,end
[str,n,l,f]=lasterror(%t);
assert_checkequal(l, 2);
assert_checkequal(f, "foo");
