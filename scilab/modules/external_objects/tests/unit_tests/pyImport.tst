// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================

pyAutounwrap(%f);
pyImport urllib;
// a variable named urllib has been created
s = urllib.quote("hello world");

assert_checkequal(pyUnwrap(s), 'hello%20world');
assert_checktrue( pyUnwrap(s == pyWrap('hello%20world')));

pyAutounwrap(%t);
s = urllib.quote("hello world");
assert_checkequal(s, 'hello%20world');
assert_checktrue( s == 'hello%20world');

foo = pyImport("urllib2", %f)
t = foo.unquote(s);
assert_checkerror("foo.unquote(42)",[],999);
pyRemove foo

assert_checkequal(t, 'hello world');
assert_checktrue(t == 'hello world');

pythonMath = pyImport("math", %f)
assert_checkerror("pythonMath.ceil()",[],999);
assert_checkerror("pythonMath.ceil(''aze'')",[],999);
a=pythonMath.ceil(2.5);
assert_checkequal(a,3);

oldPwd=pwd();
cd TMPDIR;
fd = mopen('plop.py','wt');
mputl('def foo():',fd);
mputl('	   return 42',fd);
mclose(fd);
plopPkg = pyImport("plop", %f);
chdir(oldPwd);
assert_checktrue(plopPkg.foo()==42);
assert_checkequal(plopPkg.foo(),int32(42));
pyRemove plopPkg

pyEvalStr("execfile("""+TMPDIR+"/plop.py"")")
main=pyMain(); 
assert_checktrue(main.foo()==42);
assert_checkequal(main.foo(),int32(42));

pyRemove urllib s foo t
