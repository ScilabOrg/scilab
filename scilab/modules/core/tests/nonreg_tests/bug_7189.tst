// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 7189 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7189
//
// <-- Short Description -->
// In some cases the usage of resume may lead to a crash of Scilab
add_profiling("generic_i_hm")
T=[];T(2,2,2)='xx';
if T(2,2,2)<>'xx' then pause,end

