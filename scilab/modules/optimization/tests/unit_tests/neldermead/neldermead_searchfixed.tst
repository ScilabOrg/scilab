// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->

//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

function [ y , index ] = squarefun ( x , index )
y = x(1)^2+x(2)^2;
endfunction

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",0.5);
nm = neldermead_configure(nm,"-method","fixed");
nm = neldermead_configure(nm,"-function",squarefun);
nm = neldermead_search(nm);
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0;0.0], 1e-6 );
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , 0.0, 1e-6 );
nm = neldermead_destroy(nm);

//
// Interrupt the algorithm when done.

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-method","fixed");
nm = neldermead_configure(nm,"-function",squarefun);
nm = neldermead_configure(nm,"-verbose",1);
// Set the relative size to zero: it should never stop...
nm = neldermead_configure(nm,"-tolsimplexizerelative",0.0);
nm = neldermead_search(nm);
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0;0.0], 1e-6 );
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , 0.0, 1e-6 );
nm = neldermead_destroy(nm);

