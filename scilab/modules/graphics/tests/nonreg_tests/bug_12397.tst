// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 12397 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12397
//
// <-- Short Description -->
// Demo for hist3d is not localized

assertcheck_true(getlanguage()~='en_US');
hist3d();
//Check that names of french cities are in the selected language
