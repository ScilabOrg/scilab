// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 8810 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8810
//
// <-- Short Description -->
//Using hypermatrix as index in extraction operations creates an error
a=2;
if or(a(ones(2,2,2))<>2*ones(2,2,2)) then pause,end

//see also unit_tests/Extraction.tst for a extensive test of extraction operations

