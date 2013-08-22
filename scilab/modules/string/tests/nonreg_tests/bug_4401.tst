// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 4401 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4401
//
// <-- Short Description -->
// isnum returns wrong values

//========================================================================================
r = isnum("%pi");
if r <> %t then pause,end
r = isnum("bla_bla");
if r <> %f then pause,end
r = isnum("pi+1");
if r <> %f then pause,end
r = isnum("%pi+1");
if r <> %t then pause,end
r = isnum("%nan");
if r <> %t then pause,end
r = isnum("%inf");
if r <> %t then pause,end
r = isnum("%e");
if r <> %t then pause,end
r = isnum("%e+1");
if r <> %t then pause,end
r = isnum("%e+%pi");
if r <> %t then pause,end
r = isnum("%t");
if r <> %f then pause,end
r = isnum("%i");
if r <> %t then pause,end
r = isnum("1+%i");
if r <> %t then pause,end
r = isnum("1+%2i");
if r <> %f then pause,end
r = isnum("1+%2*i");
if r <> %f then pause,end
r = isnum("1+%i*2");
if r <> %t then pause,end
r = isnum("1+2*%i");
if r <> %t then pause,end
//========================================================================================


