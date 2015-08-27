//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 743 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=743
//
// <-- Short Description -->
//    Le code suivant tourne en scilab 2.7.2 mais pas en 3.0:
//    x = int32(253);
//    if ((x&int32(1))<>int32(0)) then
//      printf('Test\n');
//    end

x = int32(253);
if ((x&int32(1)) == int32(0)) then pause,end
