// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2661 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2661
//
// <-- Short Description -->
//   gsort() doesn't process int32 value anymore.

A = [ 1 3 5 ; 8 7 8 ]

B = int8(A);
C = uint8(A);
D = int16(A);
E = uint16(A);
F = int32(A);
G = uint32(A);

A_sorted = [ 8  7  3 ; 8  5  1 ]

B_sorted = int8(A_sorted);
C_sorted = uint8(A_sorted);
D_sorted = int16(A_sorted);
E_sorted = uint16(A_sorted);
F_sorted = int32(A_sorted);
G_sorted = uint32(A_sorted);

if or( gsort(B) <> B_sorted ) then pause, end
if or( gsort(C) <> C_sorted ) then pause, end
if or( gsort(D) <> D_sorted ) then pause, end
if or( gsort(E) <> E_sorted ) then pause, end
if or( gsort(F) <> F_sorted ) then pause, end
if or( gsort(G) <> G_sorted ) then pause, end
