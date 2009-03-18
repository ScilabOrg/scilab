// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3773 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3773
//
// <-- Short Description -->
// bug in predef "ans" was protected

ierr = execstr("predef(''a'');predef(''a'')","errcatch");
if ierr <> 0 then pause,end
