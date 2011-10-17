//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

// <-- JVM NOT MANDATORY -->

// <-- ENGLISH IMPOSED -->

function z = %ZDYF4WCD_acosh(x)
  v = acosh(x.v)
  dv = 1 ./ sqrt(x.v.^2-1).*x.dv
  z = mlist(["ZDYF4WCD";"v";"dv"],v,dv)
endfunction

x = mlist(["ZDYF4WCD";"v";"dv"],0.5,0.5);
t = overloading_funeval("acosh",x);
assert_checkequal(typeof(t),"ZDYF4WCD");
assert_checkalmostequal(t.v,1.047197551196597853*%i);
assert_checkalmostequal(t.dv,-5.773502691896258421e-1*%i);
