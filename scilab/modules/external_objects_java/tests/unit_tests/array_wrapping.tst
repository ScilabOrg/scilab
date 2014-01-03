// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

c = jcompile("Test", ["public class Test {"
"public static double foo(double[] x) {"
" double s = 0;"
" for (int i = 0; i < x.length; i++) s += x[i];"
" return s;"
"}"
"public static double foo(String[] x) {"
" double s = 0;"
" for (int i = 0; i < x.length; i++) s += x[i].length();"
" return s;"
"}"
"public static double bar(int[] x) {"
" double s = 0;"
" for (int i = 0; i < x.length; i++) s += x[i];"
" return s;"
"}"
"}"
]);
jimport Test;

x = 123;
assert_checkequal(Test.foo(x), x);

x = rand(1,100);
assert_checkequal(Test.foo(x), sum(x));

x = rand(100,100);
assert_checkequal(Test.foo(x), sum(x));

assert_checkequal(Test.foo(1,2,3,4,5,6), sum(1:6));

x = "hello";
assert_checkequal(Test.foo(x), length(x));

x = string(rand(1,100));
assert_checkequal(Test.foo(x), sum(length(x)));

x = string(rand(100,100));
assert_checkequal(Test.foo(x), sum(length(x)));

x = 123;
assert_checkequal(Test.bar(x), x);

x = floor(123 * rand(1,100));
assert_checkequal(Test.bar(x), sum(x));

assert_checkequal(Test.bar(1,2,3,4,5,6), sum(1:6));