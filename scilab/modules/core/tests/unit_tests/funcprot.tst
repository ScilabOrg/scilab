//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// funcprot

deff("foo()", "a = 1");

funcprot(0);
assert_checkequal(funcprot(), 0);
assert_checkequal(execstr("deff(""foo()"", ""a = 1"");", "errcatch"), 0);
deff("foo()", "a = 2");

funcprot(1);
assert_checkequal(funcprot(), 1);
assert_checkequal(execstr("deff(""foo()"", ""a = 3"");", "errcatch"), 0);
deff("foo()", "a = 4")

funcprot(2);
assert_checkequal(funcprot(), 2);
assert_checkfalse(execstr("deff(""foo()"", ""a = 5"");", "errcatch") == 0);
assert_checkequal(execstr("deff(""foo()"", ""a = 4"");", "errcatch"), 0);


// No warning in new scope
funcprot(1);
clear foo;
function foo()
    deff("g", "j");
endfunction
function foo2()
    deff("g", "");
    foo();
endfunction

foo2() // Must not show a warning
