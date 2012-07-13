// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7231 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7231
//
// <-- Short Description -->
// mtlb_num2str did not manage second input argument

if execstr("mtlb_num2str(2, ""%11.4f"")", "errcatch") <> 0 then pause, end
if execstr("mtlb_num2str(2, ""%11.4f"", 3)", "errcatch") <> 58 then pause, end
if execstr("mtlb_num2str(2, ""blabla"")", "errcatch") <> 999 then pause, end

if mtlb_num2str(2, "%11.4f") <> "     2.0000" then pause, end
if mtlb_num2str(2) <> "2" then pause, end
