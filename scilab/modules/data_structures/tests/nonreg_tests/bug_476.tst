// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 476 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=476
//
// <-- Short Description -->
//    Bug Report Id: 120442003624185651
//
//
//    On Scilab CVS with " %hm_find " function
//    The Error Messages are:
//      wrong result
//    Commands:  -->a=hypermat([3,3,1],int8(1:9));
//
//    -->[i,j]=find(a==5)   
//     j  =
//
//        1.  
//     i  =
//
//        0.  
//
//    -->[i,j,k]=find(a==5)
//     k  =
//
//        1.  
// ...

//correct=%F
a       = hypermat([3,3,1],int8(1:9));
[i,j]   = find(a==5);
[k,l,m] = find(a==5);

assert_checkequal(m, 1);
assert_checkfalse((i==k)<>(j==l));
