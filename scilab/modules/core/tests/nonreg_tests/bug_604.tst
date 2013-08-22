// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 604 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=604
//
// <-- Short Description -->
//    -->g=9.81;m=2;l=1;K=10;I=12;
//    -->M=[g*m*l,I+2m*L^2,K/l;0,1,0;0,0,1]
//
//    returns a VERY confusing error message.
//
//    The bug appears also in
//    -->[2m]  which returns
//     ans  =
//
//    !   2.    2. !
//    while
//    -->2m  returns
//      !--error    40
//    waiting for end of command
//    and
//    -->(2m) returns
//         !--error     3
//    waiting for right parenthesis
//    as well as
//    -->(2m))))))))))))
// ...

g=9.81;
m=2;
l=1;
K=10;
I=12;

//M=[g*m*l,I+2*m*l^2,K/l;0,1,0;0,0,1];

//Le bug n'existe plus, la possibilit� de faire 2m au lieu de 2*m a �t� supprim�e

if [2*m] <> 4 then pause,end
if  2*m  <> 4 then pause,end
if (2*m) <> 4 then pause,end
