// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014-2014 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// test de creation des listes avec des elements null()

l = list(,);
m = l(1);
n = l(2);
assert_checkfalse(isdef("m"));
assert_checkfalse(isdef("n"));

l = list(1,);
n = l(2);
assert_checkfalse(isdef("n"));
assert_checkequal(l(1), 1);

l = list(,1);
m = l(1);
assert_checkfalse(isdef("m"));
assert_checkequal(l(2), 1);

l = list(,,);
m = l(1);
n = l(2);
p = l(3);
assert_checkfalse(isdef("m"));
assert_checkfalse(isdef("n"));
assert_checkfalse(isdef("p"));

l = list(1,,);
n = l(2);
p = l(3);
assert_checkequal(l(1), 1);
assert_checkfalse(isdef("n"));
assert_checkfalse(isdef("p"));

l = list(,1,);
m = l(1);
p = l(3);
assert_checkfalse(isdef("m"));
assert_checkequal(l(2), 1);
assert_checkfalse(isdef("p"));

l = list(,,1);
m = l(1);
n = l(2);
assert_checkfalse(isdef("m"));
assert_checkfalse(isdef("n"));
assert_checkequal(l(3), 1);

l = list(1,1,);
p = l(3);
assert_checkequal(l(1), 1);
assert_checkequal(l(2), 1);
assert_checkfalse(isdef("p"));

l = list(1,,1);
n = l(2);
assert_checkequal(l(1), 1);
assert_checkfalse(isdef("n"));
assert_checkequal(l(3), 1);

l = list(,1,1);
m = l(1);
assert_checkfalse(isdef("m"));
assert_checkequal(l(2), 1);
assert_checkequal(l(3), 1);