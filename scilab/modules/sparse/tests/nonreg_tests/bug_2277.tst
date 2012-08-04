// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2277 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2277
//
// <-- Short Description -->
//    Scilab has a bug with the SPARSE command. When the "values vector" in the
//    row/column representation is COMPLEX the routine fails to build the sparse
//    matrix correctly. This seems to occur always when the first AND third entry of
//    this vector are zero.
//    The other bug is that when other elements are zero (not the third, the fifth for
//    example) they are still stored in the sparse representation.
//    This is very problematic for people doing finite element calculations with scilab!

ij          = [1,3;1,7;3,3;4,4;5,3;6,10;7,1;7,9;8,10;9,6;9,7;9,9;10,10] ;
v           = [1 0 0 4:13]'*%i;
A           = sparse(ij,v,[10 10]);
[ij2,v2,mn] = spget(A);

if or(v2<>[1 4:13]'*%i) then pause,end

ij(2:3,:)   = [];
if or(ij<>ij2) then pause,end
