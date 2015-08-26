// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 2345 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2345
//
// <-- Short Description -->
// Parser bug: "=>" interpreted as "=" in comparisons (even with spacing)

// <-- CLI SHELL MODE -->

if execstr("1=>0", "errcatch") == 0 then pause,end
if execstr("1=>1", "errcatch") == 0 then pause,end
if execstr("1=>2", "errcatch") == 0 then pause,end
if execstr("1 = > 2", "errcatch") == 0 then pause,end