// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1057 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1057
//
// <-- Short Description -->
//    Crash when entering the following in the command window:
//
//    1.7977E+308
//
//    Actually, every number greater than 1.797E+308 crashes
//    scilab (scilab3.0 under linux gives the correct answer Inf).
//
//    A popup window appears with the following message:
//
//    forrtl: severe(64): input conversion error, unit -5, file
//    Internal List Directed Read
//    Image [...]
//    LibScilab.dll [...]
//    [...]
//
//    [...] is text I don't think is important.
//
//    Basically overflow is translated in crashes where it should
//    be Inf.

if 1.7977E+308 <> %inf then pause,end
