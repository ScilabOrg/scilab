// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13503 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13503
//
// <-- Short Description -->
// Segfault with xarrows

xarrows([0 ; 1], [0 ; 0], [0 ; 0]);