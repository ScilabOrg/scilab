// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 8000 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8000
//
// <-- Short Description -->
// Call to 'error' or 'get' with C tagged string caused scilab crash.

f = gcf();
if execstr("get(f,""%s %s %s %s"")", "errcatch") <> 999 then pause, end
le = lasterror();
if le <> msprintf(_("Unknown property: %s.\n"), "%s %s %s %s") then pause, end

if execstr("error(1001, ""%s %d %f"")", "errcatch") <> 1001 then pause, end
le = lasterror();
if le <> "%s %d %f" then pause; end;

if execstr("error( ""%s %s %s"")", "errcatch") <> 10000 then pause, end
le = lasterror();
if le <> "%s %s %s" then pause; end;
