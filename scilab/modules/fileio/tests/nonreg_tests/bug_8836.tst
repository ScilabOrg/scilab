// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 8836 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8836
//
// <-- Short Description -->
// fileparts crashed scilab when matrix of strings was passed as input argument.
//

if execstr("fileparts(ls(""SCI/modules/fileio/macros/*.sci""))", "errcatch") <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Wrong size for input argument #%d: A string expected."), "fileparts", 1) then pause, end