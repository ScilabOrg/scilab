// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6437 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6437
//
// <-- Short Description -->
// fileext('plop') crashed Scilab on linux

if fileext('plop') <> '' then pause,end
if fileext('') <> '' then pause,end
if fileext('.tata.toto') <> '.toto' then pause,end
