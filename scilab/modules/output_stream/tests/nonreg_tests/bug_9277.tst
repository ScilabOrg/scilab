// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9277 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9277
//
// <-- Short Description -->
// escape character "\" did not work with msprintf.
//

if msprintf("\tab") <> ascii(9) + "ab" then pause, end
if msprintf("\\tab") <> "\tab" then pause, end
if msprintf("\\\") <> "\" then pause, end
if msprintf('|%s|','\tab') <> "|\tab|" then pause, end
