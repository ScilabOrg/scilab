// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


myString=jwrap("foo");
assert_checkequal(jgetclassname(myString),"java.lang.String");

a=jwrap(2);
assert_checkequal(typeof(a),"_EObj");
assert_checkequal(jgetclassname(a),"double");
b=junwrap(a);
assert_checkequal(typeof(b),"constant");


c = jcompile("Test", ["public class Test {";
    "public int[] returnArrayInt() {";
    "    return new int[]{1,2,3,4};";
    "}";
    "}";]);
t = c.new();
assert_checkequal(jgetclassname(c),"Test");
t.returnArrayInt()
