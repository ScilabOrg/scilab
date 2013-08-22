// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1712 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1712
//
// <-- Short Description -->
//    Logical binary operations &,| with hypermatrices fail, as shown in the
//    following examples. (The unary operation ~ succeeds.)
//
//    -->hm=hypermat([2 2 2],1:8)
//
//    -->hm>min(hm) & hm<max(hm)
//                            !--error 4
//    undefined variable : %l_h_l
//
//    -->hm==min(hm) | hm==max(hm)
//                              !--error 4
//    undefined variable : %l_g_l
//
//    -->~(hm==min(hm))
//     ans  =
//
//    (:,:,1)
//
//    ! F T !
//    ! T T !
// ...

//hm = hypermat([2 2 2],1:8);
//if execstr('hm>min(hm) & hm<max(hm)','errcatch')   <>0 then pause,end
//if execstr('hm==min(hm) | hm==max(hm)','errcatch') <>0 then pause,end
