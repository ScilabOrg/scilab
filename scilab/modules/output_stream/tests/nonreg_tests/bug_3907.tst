// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3907 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3907
//
// <-- Short Description -->
// msprintf invalid format can kill scilab

command_test = "msprintf(''%s%d%s%s'',''r'',''e'',''w'',1);";
ierr = execstr(command_test , "errcatch");
if ierr <> 246 then pause,end

ierr = execstr("msprintf(''%s%d%s%s'',''r'',1,''e'',''w'');","errcatch");
if ierr <> 0 then pause,end

