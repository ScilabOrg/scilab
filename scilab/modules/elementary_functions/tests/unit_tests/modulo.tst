// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for modulo() function
// =============================================================================

assert_checkerror("modulo()", msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"modulo", 2));
assert_checkerror("modulo(''a'',1)", msprintf(_("%s: Wrong type for input argument #%d: A real expected.\n"), "modulo", 1));
assert_checkerror("modulo(1,''a'')", msprintf(_("%s: Wrong type for input argument #%d: A real expected.\n"), "modulo", 2));
assert_checkerror("modulo(%pi+%i,1)", msprintf(_("%s: Wrong type for input argument #%d: A real expected.\n"), "modulo", 1));
assert_checkerror("modulo(1,1+%i)", msprintf(_("%s: Wrong type for input argument #%d: A real expected.\n"), "modulo", 2));

n=[1,2,10,15];
m=[2,2,3,5];
r = modulo(n,m);
assert_checkequal(r, [1,0,1,0]);

n = 100 * rand(1,100);
m = 100 * rand(1,100);
r = modulo(n,m);
computed_r = n - m .* int (n ./ m);
assert_checkequal(r, computed_r);

assert_checkequal(modulo(%z^2,%z),0);
