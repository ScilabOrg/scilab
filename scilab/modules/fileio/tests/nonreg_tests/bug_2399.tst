// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2399 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2399
//
// <-- Short Description -->
//    msscanf crashed scilab when scanning a vector of strings for float
//    values on Linux.
// An error is now returned.

toks = ["noddy";"holden"];

refMsg = msprintf(_("%s: An error occurred: Requested type does not match data type.\n"), "sscanf");
assert_checkerror("msscanf(-1,toks,""%f"");", refMsg);
