// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->


// <-- Non-regression test for bug 3228 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3228
//
// <-- Short Description -->
// ascii('�') returns a incorrect value


if ascii('�') <> 224 then pause,end
if ascii('�') <> 231 then pause,end
if ascii('�') <> 232 then pause,end
if ascii('�') <> 233 then pause,end
if ascii('�') <> 249 then pause,end