// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 11996 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11996
//
// <-- Short Description -->
// Extend eye() to hypermatrix

ref222(:,:,1) = [1,0;0,0];
ref222(:,:,2) = [0,0;0,1];
assert_checkequal(eye(rand(2,2,2)), ref222);

ref4445(:,:,1,1) = [1,0,0,0;0,0,0,0;0,0,0,0;0,0,0,0];
ref4445(:,:,2,1) = zeros(4,4);
ref4445(:,:,3,1) = zeros(4,4);
ref4445(:,:,4,1) = zeros(4,4);
ref4445(:,:,1,2) = zeros(4,4);
ref4445(:,:,2,2) = [0,0,0,0;0,1,0,0;0,0,0,0;0,0,0,0];
ref4445(:,:,3,2) = zeros(4,4);
ref4445(:,:,4,2) = zeros(4,4);
ref4445(:,:,1,3) = zeros(4,4);
ref4445(:,:,2,3) = zeros(4,4);
ref4445(:,:,3,3) = [0,0,0,0;0,0,0,0;0,0,1,0;0,0,0,0];
ref4445(:,:,4,3) = zeros(4,4);
ref4445(:,:,1,4) = zeros(4,4);
ref4445(:,:,2,4) = zeros(4,4);
ref4445(:,:,3,4) = zeros(4,4);
ref4445(:,:,4,4) = [0,0,0,0;0,0,0,0;0,0,0,0;0,0,0,1];
ref4445(:,:,1,5) = zeros(4,4);
ref4445(:,:,2,5) = zeros(4,4);
ref4445(:,:,3,5) = zeros(4,4);
ref4445(:,:,4,5) = zeros(4,4);
assert_checkequal(eye(zeros(4,4,4,5)), ref4445);

M = hypermat([2 3 2 2],1:24);
ref2322(:,:,1,1) = [1,0,0;0,0,0];
ref2322(:,:,2,1) = zeros(2,3);
ref2322(:,:,1,2) = zeros(2,3);
ref2322(:,:,2,2) = [0,0,0;0,1,0];
assert_checkequal(eye(M), ref2322);
// Error messages
assert_checkerror("eye(4,4,4)", [], 42);
