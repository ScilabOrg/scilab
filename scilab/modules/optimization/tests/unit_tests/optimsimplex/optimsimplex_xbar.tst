// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->

// iexcl by default
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 );
assert_checkalmostequal ( cen , [-1.2 1.5], %eps );
s1 = optimsimplex_destroy ( s1 );

// Case iexcl = 2
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 , 2 );
assert_checkalmostequal ( cen , [-0.7 1.0], %eps );
s1 = optimsimplex_destroy ( s1 );

// Case iexcl = 2:3
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 , 2:3 );
assert_checkalmostequal ( cen , [-1.2 1.0], %eps );
s1 = optimsimplex_destroy ( s1 );
//
// Test with 5 vertices and default exclusion
//
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
93.6 0.0 0.0
93.6 10.0 10.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 );
assert_checkalmostequal ( cen , [-0.65 1.0], %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with 5 vertices and several exclusions
//
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
93.6 0.0 0.0
93.6 10.0 10.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 , [1 3 5]);
assert_checkalmostequal ( cen , [-0.6    1.0], 10 * %eps );
s1 = optimsimplex_destroy ( s1 );

