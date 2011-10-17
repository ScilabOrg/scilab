// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// Check that overloading is possible for the acosh function.
function z = %ZDYF4WCD_acosh(x)
  v = acosh(x.v)
  dv = 1 ./ sqrt(x.v.^2-1).*x.dv
  z = mlist(["ZDYF4WCD";"v";"dv"],v,dv)
endfunction

function b = %ZDYF4WCD_isreal(x)
  b = isreal(x.v)
endfunction

function y = %ZDYF4WCD_mini(x)
  v = min(x.v)
  dv = zeros(v) // absolutely wrong, but runs !
  y = mlist(["ZDYF4WCD";"v";"dv"],v,dv)
endfunction

function y = %ZDYF4WCD_imag(x)
  v = imag(x.v)
  dv = zeros(v) // absolutely wrong, but runs !
  y = mlist(["ZDYF4WCD";"v";"dv"],v,dv)
endfunction

function y = %ZDYF4WCD_imult(x)
  v = imult(x.v)
  dv = zeros(v) // absolutely wrong, but runs !
  y = mlist(["ZDYF4WCD";"v";"dv"],v,dv)
endfunction

function z = %ZDYF4WCD_1_s(x,y)
  z = (x.v < y)
endfunction

function z = %ZDYF4WCD_acos(x)
  v = acos(x.v)
  dv = -1 ./ sqrt(1-x.v.^2).*x.dv
  y = mlist(["ZDYF4WCD";"v";"dv"],v,dv)
endfunction

function z = %ZDYF4WCD_2_s(x,y)
  z = (x.v > y)
endfunction

function z = %s_x_ZDYF4WCD (x,y)
  v = x.*y.v
  dv = x.*y.dv
  z = mlist(["ZDYF4WCD";"v";"dv"],v,dv)
endfunction

x = mlist(["ZDYF4WCD";"v";"dv"],0.4,0.1);
y = acosh(x);
assert_checkequal(typeof(y),"ZDYF4WCD");
assert_checkalmostequal(y.v,4.e-1*%i);
assert_checkalmostequal(y.dv,0);

