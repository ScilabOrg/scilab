// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7014 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10144
//
// <-- Short Description -->
// Cannot have a module with a space in its path

// launch scilab

//d = "/contrib/tool\ test"
cd TMPDIR
mkdir("tool test")
copyfile(SCI+"/contrib/toolbox_skeleton",  "tool test")
cd("tool test")

a = TMPDIR+"/tool test/"
refMsg = msprintf(_("%s: Wrong path for %s: No spaces allowed in module path.\n"),"tbx_builder_src",a);

assert_checkerror("exec(""builder.sce"")",refMsg);
