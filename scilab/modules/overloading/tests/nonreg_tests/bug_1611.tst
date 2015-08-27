//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1611 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1611
//
// <-- Short Description -->
// untrapped incorrect assignment
my_matrix(1, :) = [4, 5, 6];
my_list = list(1, 2, 3);

if execstr('my_matrix(1)=mylist','errcatch')==0 then pause,end;
if execstr('my_matrix(1,:)=mylist','errcatch')==0 then pause,end;
