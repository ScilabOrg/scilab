// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// unit tests for ismatrix function
// =============================================================================

// Constants
assert_checkequal(ismatrix([1 2 3]), %f);
assert_checkequal(ismatrix([1 2 3]'), %f);
assert_checkequal(ismatrix([1 2 3; 1 2 3]), %t);
assert_checkequal(ismatrix(1), %f);

// Strings
assert_checkequal(ismatrix(["s" "t" "u"]), %f);
assert_checkequal(ismatrix(["s" "t" "u"]'), %f);
assert_checkequal(ismatrix(["s" "t" "u";"s" "t" "u"]), %t);
assert_checkequal(ismatrix("s"), %f);

// Polynomials
s=poly(0,"s");
assert_checkequal(ismatrix([1+s 2+s]), %f);
assert_checkequal(ismatrix([1+s 2+s]'), %f);
assert_checkequal(ismatrix([1+s 2+s 3+s; 3+s 4+s 5+s]), %t);
assert_checkequal(ismatrix(1+s), %f);

// Booleans
assert_checkequal(ismatrix([%t %f]), %f);
assert_checkequal(ismatrix([%t %f]'), %f);
assert_checkequal(ismatrix([%t %f %t; %f %t %f]), %t);
assert_checkequal(ismatrix(%t), %f);

// Sparses
assert_checkequal(ismatrix(sparse([1 0 2])), %f);
assert_checkequal(ismatrix(sparse([1 0 2])'), %f);
assert_checkequal(ismatrix(sparse([1 0 3; 2 0 1])), %t);
assert_checkequal(ismatrix(sparse(0)), %f);

// Error messages
errmsg1 = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"ismatrix", 1);
assert_checkerror("ismatrix()", errmsg1);
errmsg2 = msprintf(_("Wrong number of input arguments."));
assert_checkerror("ismatrix(1,2)", errmsg2);
errmsg3 = msprintf(_("Wrong number of output arguments.\n"));
assert_checkerror("[r,b]=ismatrix([1 2 3]);", errmsg3);
