// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13721 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13721
//
// <-- Short Description -->
//
// Could not write in a file opened in r+ mode

fd = mopen(TMPDIR + "/bug_13721.txt", "a");
mfprintf(fd, "%s\n", "hello");
mclose(fd);
fd = mopen(TMPDIR + "/bug_13721.txt", "r+");
mfprintf(fd, "%s\n", "world");
mclose(fd);

