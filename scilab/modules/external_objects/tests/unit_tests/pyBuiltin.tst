// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

py = pyBuiltin()
mv = py.memoryview("Hello World !");
l = mv.tolist()
assert_checkequal(pyUnwrap(l),int32([72,101,108,108,111,32,87,111,114,108,100,32,33]));
s = py.sorted(l);
assert_checkequal(pyUnwrap(s),int32([32,32,33,72,87,100,101,108,108,108,111,111,114]));

py_chr = pyGetAttr(py, "chr");
m = py.map(py_chr, s);
assert_checkequal(pyUnwrap(m),[" "," ","!","H","W","d","e","l","l","l","o","o","r"]);
S = pyWrap("");

str = pyUnwrapRem(S.join(m));
assert_checkequal(str,"  !HWdellloor");
pyRemove py mv l s py_chr m S
