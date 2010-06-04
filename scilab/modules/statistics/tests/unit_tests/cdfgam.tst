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
// and the inverse CDF of gamma distribution
// CDF(x) = gamma(shape, x * scale) / Gamma(shape)
// where gamma() is the lower incomplete gamma function and
// Gamma() is the gamma function

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

// Test function for the CDF of gamma distribution
function gamcdf_test(X, Shape, Scale, Expected, precision)
    global error_count;
    n = length(X);
    if n == length(Shape) & n == length(Scale) & n == length(Expected) then
        Computed = cdfgam("PQ", X, Shape, Scale);
        for i = 1 : n
            flag = assert_close(Computed(i), Expected(i), precision);
            if flag == 0 then
                disp("Error found in gamcdf_test()!");
                printf(" x = %.16e\n", X(i));
                printf(" shape = %.16e\n", Shape(i));
                printf(" scale = %.16e\n", Scale(i));
                printf(" computed = %.16e\n", Computed(i));
                printf(" expected = %.16e\n", Expected(i));
                error_count = error_count + 1;
            end
        end
    else
        disp("Number of parameters do not match in gamcdf_test()!");
        disp("Test unfinished.");
        abort;
    end
endfunction

// Test points for the CDF of gamma distribution
X = [0.0, 5.0, 1.083, 1e+10, %pi, 1e+49, 1e-50]';
Shape = [1.0, 1.0, 5.4, 10, %e, %e, 0.5772156649]';
Scale = [2.0, 0.5, %pi, 0.1, 1e-50, 1e-50, 1e+51]';
Expected = [0.0,
            0.917915001376101158392373235984,
            0.198258786768570449687842938147,
            1.0,
            6.42404842175291493841421134073e-136,
            0.000417413448775490344110072893358,
            0.999989306565807090088071618084];
gamcdf_test(X, Shape, Scale, Expected, eps);

// Test function for the inverse CDF of gamma distribution
function gaminv_test(P, Shape, Scale, Expected, precision)
    global error_count;
    n = length(P);
    if n == length(Shape) & n == length(Scale) & n == length(Expected) then
        Q = 1 - P;
        Computed = cdfgam("X", Shape, Scale, P, Q);
        for i = 1 : n
            flag = assert_close(Computed(i), Expected(i), precision);
            if flag == 0 then
                disp("Error found in gaminv_test()!");
                printf(" p = %.16e\n", P(i));
                printf(" shape = %.16e\n", Shape(i));
                printf(" scale = %.16e\n", Scale(i));
                printf(" computed = %.16e\n", Computed(i));
                printf(" expected = %.16e\n", Expected(i));
                error_count = error_count + 1;
            end
        end
    else
        disp("Number of parameters do not match in gaminv_test()!");
        disp("Test unfinished.");
        abort;
    end
endfunction

// Test points for the inverse CDF of gamma distribution
P = [0.0,
     0.917915001376101158392373235984,
     0.198258786768570449687842938147,
     6.42404842175291493841421134073e-136,
     0.000417413448775490344110072893358,
     0.999989306565807090088071618084];
Shape = [1.0, 1.0, 5.4, %e, %e, 0.5772156649]';
Scale = [2.0, 0.5, %pi, 1e-50, 1e-50, 1e+51]';
Expected = [0.0, 5.0, 1.083, %pi, 1e+49, 1e-50]';
gaminv_test(P, Shape, Scale, Expected, eps);

printf("\n cdfgam.tst finished, %d error(s) found.\n", error_count);
