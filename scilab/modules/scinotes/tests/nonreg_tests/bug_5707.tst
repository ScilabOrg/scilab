// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5707 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5707
//
// <-- Short Description -->
// cmd-e executed the wrong editor file

edit tic
edit toc
edit calendar

// select toc & cmd-e  toc.sci must be executed
// select calendar & cmd-e  calendar.sci must be executed
// select tic & cmd-e  tic.sci must be executed



