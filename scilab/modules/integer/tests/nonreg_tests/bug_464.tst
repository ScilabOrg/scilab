//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 464 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=464
//
// <-- Short Description -->
//    Bug Report Id: 12011200362191319
//     "1" is a double, and inttype is not defined for a double argument, according to the help. However, the error message is confusing; moreover, imho, it would be proper for inttype(double) to return 0, so as to be fully compatible with iconvert().
//
//    On Scilab CVS with " inttype " function
//    The Error Messages are:
//      -->inttype(1)
//               !--error    44
//    ***th argument is incorrect
//
//    Commands:
//
//    Enrico Segre on Linux version RH 9 distribution RH 9 with  gnome as window manager
//    Israel  July 21, 2003 at 9:13:19

if execstr("inttype(1)","errcatch","n") <> 0 then pause,end
if inttype(1)<>0 then pause,end
