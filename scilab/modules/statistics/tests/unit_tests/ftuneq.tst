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

expected_p = 0.9602320;

assert_checktrue(norm(p-expected_p) < prec);

prec = 1.e-32;

T = [
107.8681568
107.8681465
107.8681572
107.8681785
107.8681446
107.8681903
107.8681526
107.8681494
107.8681616
107.8681587
107.8681519
107.8681486
107.8681419
107.8681569
107.8681508
107.8681672
107.8681385
107.8681518
107.8681662
107.8681424
107.8681360
107.8681333
107.8681610
107.8681477
];

[f,p]=ftuneq(T,T*(1+1.e-6));
expected_p = 6.033347845799172588D-31;

assert_checktrue(norm(p-expected_p) < prec);
