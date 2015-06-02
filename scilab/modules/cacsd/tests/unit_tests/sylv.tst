// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Run with test_run('cacsd', 'sylv', ['no_check_error_output']);

A = rand(4, 4); C = rand(4, 3); B = rand(3, 3);

// Solve continuous-time Sylvester equation: A*X+X*B = C
X = sylv(A, B, C, "c");

expected_norm = 1.D-16;

if norm(A*X+X*B-C)-expected_norm > 1.D-15 then pause,end

// Solve modified discrete-time Sylvester equation: A*X*B+X = C
X = sylv(A, B, C, "d");

if norm(A*X*B+X-C)-expected_norm > 1.D-15 then pause,end

// Solve real discrete-time Sylvester equation: A*X*B+X = C
X = -sylv(-A, B, C, "d");

if norm(A*X*B-X-C)-expected_norm > 1.D-15 then pause,end
