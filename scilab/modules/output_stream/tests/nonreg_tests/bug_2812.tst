// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 2812 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2812
//
// <-- Short Description -->
// I know the name of my function is too long but I should not have "ÿÿÿ" at the end.
// 
// Processing file ilib_compile.sci
// Warning :
// The identifier : generatecompilationflags_unixÿÿÿ
//  has been truncated to: generatecompilationflags

generatecompilationflags_unix = 1;
