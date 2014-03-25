// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
// Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

array = jarray("double", 10);
for i=1:10, array(i) = i; end

assert_checkequal(array(5), 5);
assert_checkequal(array([4 8 2]), [4 8 2]);
assert_checkequal(array([4;8;2]), [4 8 2]);

///////////////////////////////////////////////////////////////////////////////
jimport java.util.ArrayList;
list = ArrayList.new();
for i=1:10, list(i) = i; end

assert_checkequal(list(5), 5);
assert_checkequal(list([4 8 2]), [4 8 2]);
assert_checkequal(list([4;8;2]), [4 8 2]);

///////////////////////////////////////////////////////////////////////////////
jimport java.util.HashMap;
h = HashMap.new();
h.put("Coucou", "Foo");
h.put("Hello", "Bar");
a=rand(10,10);
h.put("MyMatrix", a);
dense=[%F, %F, %T, %F, %F
%T, %F, %F, %F, %F
%F, %F, %F, %F, %F
%F, %F, %F, %F, %T];
h.put("MyBool", dense);

assert_checkequal(h.get("Coucou"), "Foo");
assert_checkequal(h.Coucou, "Foo");
assert_checkequal(h("Coucou"),"Foo");

assert_checkequal(h.get("Hello"), "Bar");
assert_checkequal(h.Hello, "Bar");
assert_checkequal(h("Hello"),"Bar");

assert_checkequal(h.get("MyMatrix"), a);
assert_checkequal(h.MyMatrix, a);
assert_checkequal(h("MyMatrix"),a);

assert_checkequal(h.get("MyBool"), dense);
assert_checkequal(h.MyBool, dense);
assert_checkequal(h("MyBool"),dense);

