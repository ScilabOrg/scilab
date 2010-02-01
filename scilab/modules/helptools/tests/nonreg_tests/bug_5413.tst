// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 5413 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5413
//
// <-- Short Description -->
// Bad syntax is the export

docFile = help_from_sci(SCI+"/modules/helptools/tests/nonreg_tests/bug_5413.sci",TMPDIR);
fd_r = mopen(docFile,'rt');
content = mgetl(fd_r);

wrongString = "&gt;";
position = grep(content, wrongString);
// We should not get anything
if position <> [] then pause,end


wrongString = "&lt;";
position = grep(content, wrongString);
// We should not get anything
if position <> [] then pause,end
