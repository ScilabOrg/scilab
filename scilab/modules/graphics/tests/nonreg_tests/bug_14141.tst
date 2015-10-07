// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 14141 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14141
//
// <-- Short Description -->
// recursive insertion: gcf().attribute=value => "Wrong insertion : function or macro are not expected".


plot2d();
assert_checkequal(gcf().visible, "on");
gcf().visible = %f;
assert_checkequal(gcf().visible, "off");
