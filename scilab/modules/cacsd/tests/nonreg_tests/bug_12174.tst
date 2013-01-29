// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12174 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12174
//
// <-- Short Description -->
// The function "routh_t" gives incorrect output for all denominators that include
//gain value "k".

s = poly(0, 's');
k=poly(0,'k');
P=s*(s+7)*(s+11)
h=1/P;
routh_t(h,k)

