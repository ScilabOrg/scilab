// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// unit tests for isscalar function
// =============================================================================

// Constants
assert_checkequal(isscalar([1 2 3]), %f);
assert_checkequal(isscalar([1 2 3]'), %f);
assert_checkequal(isscalar([1 2; 1 2]), %f);
assert_checkequal(isscalar(1), %t);
assert_checkequal(isscalar([]), %f);

// Strings
assert_checkequal(isscalar(["s" "s"; "s" "s"]), %f);
assert_checkequal(isscalar("s"), %t);

// Polynomials
s=poly(0,"s");
assert_checkequal(isscalar([s s; s s]), %f);
assert_checkequal(isscalar(1+s), %t);

// Booleans
assert_checkequal(isscalar([%t %t; %t %t]), %f);
assert_checkequal(isscalar(%t), %t);

// Sparses
assert_checkequal(isscalar(sparse([1 2; 1 2])), %f);
assert_checkequal(isscalar(sparse(0)), %t);

// Error messages
errmsg1 = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"IsAScalar", 1);
assert_checkerror("IsAScalar()", errmsg1);
errmsg2 = msprintf(_("Wrong number of input arguments."));
assert_checkerror("IsAScalar(1,2)", errmsg2);
errmsg3 = msprintf(_("Wrong number of output arguments.\n"));
assert_checkerror("[r,b]=IsAScalar([1 2 3]);", errmsg3);
