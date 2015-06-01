// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 9600 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9600
//
// <-- Short Description -->
// Can not assign empty matrix in a list or cell

l = list();
l(1) = [];
l = list();
l1 = {};
c = {};
c{1} = [];
c{1} = {};
c{1} = list();
