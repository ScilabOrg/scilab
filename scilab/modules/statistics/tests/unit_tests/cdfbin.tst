// =============================================================================
// Copyright (C) 2010 - Michael Zhang
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// =============================================================================

funcprot(0);

// Test script for the CDF (Cumulative Distribution Function)
// and the inverse CDF of binomial distribution
// CDF(k) = beta(1 - p, n - k, k + 1) / Beta(n - k, k + 1)
// where beta() is the incomplete beta function and
// Beta() is the beta function

global error_count;
error_count = 0;

// Assert if the relative error of computed and expected is less than epsilon
function flag = assert_close(computed, expected, epsilon)
    if expected == 0.0 | computed == 0.0 then
        error = norm(computed - expected);
    else
        error = norm(computed - expected) / norm(expected);
    end
    if error < epsilon then
        flag = 1;
    else
        flag = 0;
    end
endfunction

// Tolerance of error
// Desired precision: 12 to 16 significant digits
eps = 10000 * %eps;         // %eps = 2.22e-16

// Test function for the CDF of binomial distribution
function bincdf_test(S, Xn, Pr, Expected, precision)
    global error_count;
    n = length(S);
    if n == length(Xn) & n == length(Pr) & n == length(Expected) then
        Ompr = 1 - Pr;
        Computed = cdfbin("PQ", S, Xn, Pr, Ompr);
        for i = 1 : n
            flag = assert_close(Computed(i), Expected(i), precision);
            if flag == 0 then
                disp("Error found in bincdf_test()!");
                printf(" s = %.16e\n", S(i));
                printf(" xn = %.16e\n", Xn(i));
                printf(" pr = %.16e\n", Pr(i));
                printf(" computed = %.16e\n", Computed(i));
                printf(" expected = %.16e\n", Expected(i));
                error_count = error_count + 1;
            end
        end
    else
        disp("Number of parameters do not match in bincdf_test()!");
        disp("Test unfinished.");
        abort;
    end
endfunction

// Test points for the CDF of binomial distribution
S = [5, 5, 10, 0, 9, 48, 1e+100, 300, 3e+8, 500, 5e+9, 5e+49]';
Xn = [10, 10, 21, 50, 50, 50, 1e+100, 1000, 1e+9, 1000, (1e+10)+1, 1e+50]';
Pr = [0, 1, 0.5, 0.6, 0.6, 0.6, 0.7, %pi/10, %pi/10, 0.5, 0.5, 0.5]';
Expected = [1.0,
            0.0,
            0.5,
            1.26765060022823287180678257830e-20,
            1.41953857513702970810647102315e-9,
            0.999999999722490096054627883859,
            1.0,
            0.176187116862812132023208278042,
            1.42559202491708844144639509077e-203753,
            0.512612509089315659949193104694,
            0.5,
            0.5];
bincdf_test(S, Xn, Pr, Expected, eps);

// Test function for the inverse CDF of binomial distribution
function bininv_test(P, Xn, Pr, Expected, precision)
    global error_count;
    n = length(P);
    if n == length(Xn) & n == length(Pr) & n == length(Expected) then
        Q = 1 - P;
        Ompr = 1 - Pr;
        Computed = cdfbin("S", Xn, Pr, Ompr, P, Q);
        Computed = round(Computed);
        for i = 1 : n
            flag = assert_close(Computed(i), Expected(i), precision);
            if flag == 0 then
                disp("Error found in bininv_test()!");
                printf(" p = %.16e\n", P(i));
                printf(" xn = %.16e\n", Xn(i));
                printf(" pr = %.16e\n", Pr(i));
                printf(" computed = %.16e\n", Computed(i));
                printf(" expected = %.16e\n", Expected(i));
                error_count = error_count + 1;
            end
        end
    else
        disp("Number of parameters do not match in bininv_test()!");
        disp("Test unfinished.");
        abort;
    end
endfunction

// Test points for the inverse CDF of binomial distribution
P = [1.0,
     0.0,
     0.5,
     1.26765060023e-20
     9.63414456173453889048181587509e-19,
     1.41953857513702970810647102315e-9,
     0.999999999722490096054627883859,
     1.0,
     0.176187116862812132023208278042,
     0.512612509089315659949193104694,
     0.5,
     0.5];
Xn = [10, 10, 21, 50, 50, 50, 50, 1e+100, 1000, 1000, (1e+10)+1, 1e+50]';
Pr = [0, 1, 0.5, 0.6, 0.6, 0.6, 0.6, 0.7, %pi/10, 0.5, 0.5, 0.5]';
Expected = [5, 5, 10, 0, 1, 9, 48, 1e+100, 300, 500, 5e+9, 5e+49]';
bininv_test(P, Xn, Pr, Expected, eps);

printf("\n cdfbin.tst finished, %d error(s) found.\n", error_count);
