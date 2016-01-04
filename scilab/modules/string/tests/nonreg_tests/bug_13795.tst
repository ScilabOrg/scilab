// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// Non regression test for bug 13795
// grep with regular expressions options did not properly match the empty string ""
res = grep("", "/^$/","r")
assert_checkequal(res, 1);

res = grep("", "/^\s*$/","r")
assert_checkequal(res, 1);

A = [" ", "", " ", ""];
res = grep(A, "@\s*@", "r");
assert_checkequal(res, [1 2 3 4]);

A = [" ", "", " ", ""];
res = grep(A, "@\s+@", "r");
assert_checkequal(res, [1 3]);
