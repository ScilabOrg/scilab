// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 661 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=661
//
// <-- Short Description -->
//    the function quart(x) only works with an even x.
//    Although the bug is normal, took me a whole afternoon
//    of debugging of my macro to reach the conclusion
//    that there was a problem with quart.

if or(abs(quart(1:3)-[1.25;2;2.75])>=10*%eps) then pause,end
