// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug K -->
//
// <-- OLD BUG -->

x = "a";
z = x($:-1:1);

if z <> "a" then pause,end
