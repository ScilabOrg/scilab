// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 4839 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4839
//
// <-- Short Description -->
//    grand does not perform well with multivariate gaussian



A = [5 3 1;3 5 1; 1 1 5];
computed = grand(10,'mn',[0  0 0]',A);
assert_checkalmostequal ( size(computed) , [3 10] );

