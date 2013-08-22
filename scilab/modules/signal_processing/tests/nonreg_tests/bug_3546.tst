// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2005-2008 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3546 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3546
//
// <-- Short Description -->
// frmag broken in scilab 5.0
[hm1,fr1]=frmag([0.4 -1 -1],[1 0.85 -1],256);
[hm2,fr2]=frmag(poly([0.4 -1 -1],"z","c"),poly([1 0.85 -1],"z","c"),256);
if or(hm1<>hm2)|or(fr1<>fr2) then pause,end
