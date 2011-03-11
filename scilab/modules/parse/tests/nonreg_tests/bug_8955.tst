// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8955 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8955
//
// <-- Short Description -->
// 2 semantic behaviour with same syntax

a = 10

if a(1) <> 10 then pause, end

if a     (1) <> 10 then pause, end

if [a(1)] <> 10 then pause, end

if or([a    (1)] <> [10,1]) then pause, end
