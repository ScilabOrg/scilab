// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for ftuneq() function
// =============================================================================

// Run with test_run('statistics', 'ftuneq', ['no_check_error_output']);

prec = 1.e-5;

sample1 = [46 55 54];
sample2 = [53 54];
sample3 = [50 49 58 51 50];
sample4 = [61 51 46 52];

[f, p] = ftuneq(sample1, sample2, sample3, sample4);

expected_p = 0.0397680;

if norm(p-expected_p) > prec then pause,end
