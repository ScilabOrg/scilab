// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 8223 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8223
//
// <-- Short Description -->
// Loading an Xcos file after building the doc did not work

// loading the Saxon xml engine
xmltojar("modules/xcos", [], "en_US")

// launching xcos
xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_8052.xcos")
