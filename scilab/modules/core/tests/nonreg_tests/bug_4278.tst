// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4278 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4278
//
// <-- Short Description -->
// typename() called with a wrong number of input arguments  returns incoherent errors.


ierr = execstr("typename(''hgh'')","errcatch");
if ierr <> 999 then pause,end

ierr = execstr("typename([17])","errcatch");
if ierr <> 999 then pause,end

typename("my type",100)
ierr = execstr("typename(''my type'',110)","errcatch");
if ierr <> 999 then pause,end

