// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->
// <-- Non-regression test for bug 8000 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8000
//
// <-- Short Description -->
// Call to 'error' or 'get' with C tagged string caused scilab crash.

f=gcf();
execstr("get(f,''%s %s %s %s'')", "errcatch")
le = lasterror();
if (le <> "Unknown property: %s %s %s %s.") then pause; end;

execstr("error(1001, ''%s %d %f'')", "errcatch")
le = lasterror();
if (le <> "%s %d %f") then pause; end;
