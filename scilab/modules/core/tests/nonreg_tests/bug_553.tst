// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 553 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=553
//
// <-- Short Description -->
//    Bug Report Id: 0327120031024153915
//    Funny behavior when an interpreted function has a function argument that is called within a loop -- the loop exits after the first pass.
//
//    I don't know if it's related, but I just discovered that normal loop termination clears the loop variable:
//
//        -->n=12;
//         n =
//           12.
//
//        -->for n=1:3; disp(n); end
//           1.
//           2.
//           3.
//
//        -->n
//          !--error  4
//        undefined variable : n
//
//    Is this correct? I was expecting n == 3.
//    On Scilab 2.7.2 with " loopbug.sci, shown below " function
// ...

n = 12;
if execstr("for n=1:3; disp(n); end","errcatch") <> 0 then pause,end
