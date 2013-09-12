// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

//
// Check behaviour with default settings.
//



function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test tolerance on variance of function values
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",600);
nm = neldermead_configure(nm,"-maxfunevals",600);
nm = neldermead_configure(nm,"-tolfunmethod",%f);
nm = neldermead_configure(nm,"-tolxmethod",%f);
nm = neldermead_configure(nm,"-tolvarianceflag",%t);
nm = neldermead_configure(nm,"-tolabsolutevariance",1.e-4);
nm = neldermead_configure(nm,"-tolrelativevariance",1.e-4);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_search(nm);
// Check optimum point
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 4.0, 1e-1 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "tolvariance" );
nm = neldermead_destroy(nm);

