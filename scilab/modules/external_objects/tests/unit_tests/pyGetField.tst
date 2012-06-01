// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================

pyImport os
s = pyGetField(os, "name");
assert_checktrue(length(s)>1);

s = pyGetField(os, "name", %t);
assert_checkequal(typeof(s),"string");
assert_checktrue(length(s)>1);

pyShowPrivate(%f);
pyImport os.path;
a=pyGetFields(os.path);
assert_checktrue(size(a,"*") > 5);
pyShowPrivate(%t);
b=pyGetFields(os.path);
assert_checktrue(size(b,"*") > size(a,"*"));



pyEvalStr("execfile("""+SCI+"/modules/external_objects/tests/unit_tests/plop.py"")")
pyEnv=pyMain();
plop=pyEnv.plop();
assert_checkequal(typeof(plop.i),"int32");
assert_checkequal(plop.i,int32(12));

assert_checkequal(typeof(plop.aze()),"string");
assert_checkequal(plop.aze(),"my string");

pyShowPrivate(%f);
assert_checkequal(size(pyGetFields(plop),"*"),1);
assert_checkequal(pyGetFields(plop),"i");

pyShowPrivate(%t);
assert_checktrue(size(pyGetFields(plop),"*")>1);
assert_checkfalse(find(pyGetFields(plop)=="i")==[]);
assert_checkfalse(find(pyGetFields(plop)=="_plop__privateMember")==[]);
fields=pyGetFields(plop);
b=find(fields=="_plop__privateMember");
assert_checkequal(fields(b),"_plop__privateMember");
assert_checkequal(plop.bar2(),int32(43));
