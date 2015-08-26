// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9600 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9600
//
// <-- Short Description -->
// Can not assign empty matrix in a list or cell


l = list();
if execstr("l(1) = []", "errcatch") <> 0 then pause, end
if l(1) <> [] then pause, end

l = list();
if execstr("l(1) = {}", "errcatch") <> 0 then pause, end
if l(1) <> {} then pause, end

c = {};
if execstr("c{1} = []", "errcatch") <> 0 then pause, end
if c{1} <> [] then pause, end

if execstr("c{1} = {}", "errcatch") <> 0 then pause, end
if c{1} <> {} then pause, end

if execstr("c{1} = list()", "errcatch") <> 0 then pause, end
if c{1} <> list() then pause, end
