// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 7014 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7014
//
// <-- Short Description -->
// ilib_verbose value modified after toolbox_skeleton

// launch scilab

refMsg = msprintf(_("%s: Wrong path for %s: No spaces allowed in module path.\n"),"tbx_builder_src",module));

d = "SCI/contrib/toolbox skeleton"
cp SCI/contrib/toolbox_skeleton d
cd(d)

assert_checkerror(exec(builder.sce),refMsg);
