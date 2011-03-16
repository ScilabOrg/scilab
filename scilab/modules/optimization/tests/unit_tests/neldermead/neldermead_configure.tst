// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->

//
// Check behaviour with configured settings.
//


function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Check maxfunevals with "variable" method
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",0.1);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-maxfunevals",10);
nm = neldermead_search(nm);
funevals = neldermead_get(nm,"-funevals");
// Let's be not strict
assert_checkequal ( funevals < 15 , %T );
// Cleanup
nm = neldermead_destroy(nm);

//
// Check maxiter with "variable" method
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",0.1);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_search(nm);
iterations = neldermead_get(nm,"-iterations");
assert_checkequal ( iterations , 10 );
// Cleanup
nm = neldermead_destroy(nm);

// Wrong -method flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-method'',''foo'')";
assert_checkerror(cmd,"unknownValueForOption: Unknown value foo for -method option");
nm = neldermead_destroy(nm);

// Wrong -simplex0method flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-simplex0method'',''foo'')";
assert_checkerror(cmd,"unknownValueForOption: Unknown value foo for -simplex0method option");
nm = neldermead_destroy(nm);

// Wrong -tolsimplexizemethod flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-tolsimplexizemethod'',''foo'')";
assert_checkerror(cmd,"assert_typeboolean: Expected boolean but for variable value at input #3, got string instead.");
nm = neldermead_destroy(nm);

// Wrong -tolssizedeltafvmethod flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-tolssizedeltafvmethod'',''foo'')";
assert_checkerror(cmd,"assert_typeboolean: Expected boolean but for variable value at input #3, got string instead.");
nm = neldermead_destroy(nm);

//
// Check wrong key for get method
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",0.1);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-maxfunevals",2);
nm = neldermead_search(nm);
cmd = "funevals = neldermead_get(nm,''-foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_get: Unknown key -foo";
assert_checkequal ( computed , expected );
nm = neldermead_destroy(nm);


//
// Check that x0 is forced to be a column vector
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
cmd = "nm = neldermead_configure(nm,''-x0'',[-1.2 1.0]);";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_configure: The x0 vector is expected to be a column matrix, but current shape is 1 x 2";
assert_checkequal ( computed , expected );
nm = neldermead_destroy(nm);

//
// Check -restartstep
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
// Check that -restartstep is forced to be of consistent length : 1 or n
cmd = "nm = neldermead_configure(nm,''-restartstep'',[1 2 3]);";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_configure: The restartstep vector is expected to have 2 x 1 shape, but current shape is 1 x 3";
assert_checkequal ( computed , expected );
// Check that -restartstep is forced to be positive
cmd = "nm = neldermead_configure(nm,''-restartstep'',[-1 2]'');";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_configure: The restartstep vector is expected to be positive";
assert_checkequal ( computed , expected );
nm = neldermead_destroy(nm);


//
// Check -restarteps
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
// Check that -restarteps is forced to be a scalar double
cmd = "nm = neldermead_configure(nm,''-restarteps'',[1 2]);";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_configure: The restarteps option is expected to be a scalar, but current shape is 1 x 2";
assert_checkequal ( computed , expected );
// Check that -restarteps is forced to be positive
cmd = "nm = neldermead_configure(nm,''-restarteps'',-1);";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_configure: The restarteps option is expected to be positive";
assert_checkequal ( computed , expected );
nm = neldermead_destroy(nm);

//
// Check the display system
//
nm = neldermead_new ();
nm
nm = neldermead_destroy(nm);

