// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 7858 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7858
//
// <-- Short Description -->
// variance and variancef can take now take the a priori mean as input, and it can be a scalar.
// =============================================================================

//--------------------------------------------------------------
// Variance

x = [
0.9    0.7
0.1    0.1
0.5    0.4 ];
orien = "r";
expectedM = [1 1];
expectedV = [1.07 1.26]/3;

// Voluntarily passing a bad a priori mean, to check that we get the same results with scalar
[Variance, Mean] = variance( x, orien, ones(mean(x,orien)) );
assert_checkalmostequal([Variance Mean], [expectedV expectedM]);
// With a scalar
[Variance, Mean] = variance( x, orien, 1 );
assert_checkalmostequal([Variance Mean], [expectedV expectedM]);

orien = "c";
expectedM = [1; 1; 1];
expectedV = [0.05; 0.81; 0.305];

// Voluntarily passing a bad a priori mean, to check that we get the same results with scalar
[Variance, Mean] = variance( x, orien, ones(mean(x,orien)) );
assert_checkalmostequal([Variance Mean], [expectedV expectedM]);
// With a scalar
[Variance, Mean] = variance( x, orien, 1 );
assert_checkalmostequal([Variance Mean], [expectedV expectedM]);


//--------------------------------------------------------------
// Variancef

x = [0.2113249 0.0002211 0.6653811; 0.7560439 0.9546254 0.6283918];
fre = [1 2 3; 3 4 3];
orien = "r";
refM = [1 1 1];
refV = [0.200138037385 0.334558519179 0.125031231272];

[v, m] = variancef( x, fre, orien, ones(meanf(x,fre,orien)) );
assert_checkalmostequal([v m], [refV refM]);

[v, m] = variancef( x, fre, orien, 1 );
assert_checkalmostequal([v m], [refV refM]);

orien = "c";
refM = [1; 1];
refV = [0.492838922640; 0.060105711640];
[v, m] = variancef( x, fre, orien, ones(meanf(x,fre,orien)) );
assert_checkalmostequal([v m], [refV refM]);

[v, m] = variancef( x, fre, orien, 1 );
assert_checkalmostequal([v m], [refV refM]);
