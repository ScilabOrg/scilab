// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 12785 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12785
//
// <-- Short Description -->
// plot did not allow int data as first argument
//

plot(int8(1:2));