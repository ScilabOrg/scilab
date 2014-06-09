// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->


r = 2;
R = [1,2;3,4];
R3(:,:,1) = R;
R3(:,:,2) = R';
c = 1 + 2*%i;
C = [1+2*%i,2+4*%i;3+6*%i,4+8*%i];
C3(:,:,1) = C;
C3(:,:,2) = C';

// double == double
assert_checktrue(r == r);
assert_checktrue(c == c);

// DOUBLE == DOUBLE
assert_checktrue(R == R);
assert_checktrue(C == C);
assert_checktrue(R3 == R3);
assert_checktrue(C3 == C3);
