// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

pyUseNumpy(%t);

pyAddVar("foo",[23,2])
pyEvalStr("print foo")

pyEvalStr("A = [[1,2,3],[4,5,6]]")
pyAddVar("foo",[[1,2,3];[4,5,6]])
pyEvalStr("print A==foo")
pyEvalStr("isEqual=(A==foo)")
isEqual = pyGetVar("isEqual")

//assert_checkequal(isEqual

pyEvalStr("A = numpy.random.rand(10,8)")
a=pyUnwrap(pyGetVar("A"));

assert_checkequal(size(a), [10, 8]);
assert_checktrue(and(a<1));


pyUseNumpy(%f)
pyAddVar("foo",[23,2])
pyEvalStr("print foo");

pyRemove a
