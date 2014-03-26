// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

array = jarray("double", 10);
for i=1:10, array(i) = i; end

array(5) = 5;
assert_checkequal(array(5), 5);

array([4 8 2]) = 0;
array([4 8 2]) = [4 8 2];
assert_checkequal(array([4 8 2]), [4 8 2]);

array([4;8;2]) = 0;
array([4;8;2]) = [4 8 2];
assert_checkequal(array([4;8;2]), [4 8 2]);

///////////////////////////////////////////////////////////////////////////////
jimport java.util.ArrayList;
list = ArrayList.new();
for i=1:10, list(i) = i; end

assert_checkequal(list(5), 5);
assert_checkequal(list([4 8 2]), [4 8 2]);
assert_checkequal(list([4;8;2]), [4 8 2]);

