// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [wn,z] = damp(A,dt)
//Natural frequency and damping factor .
//   [wn,z] = damp(A) returns vectors wn and z containing the
//   natural pulsation (rd/s) and damping factors of A.   
//   The variable A can be:
//   A linear dynamical system (transfer function or state space)  
//   A real or complex array of floating point numbers
//   A real or complex array of polynomials
  dt1=[];order=%t
  select typeof(A)
  case "polynomial" then //polynomial array
    R=[];
    for k=1:size(A,'*')
      R=[R;roots(A(k),"e")];
    end
  case "constant" then //roots array
    R=A
    order=%f
  case "rational"  then //transfer function
    dt1=A.dt
    A=tf2ss(A)
    R=spec(A.A)
  case "state-space"  then 
    dt1=A.dt
    R=spec(A.A)
  else
    error(msprintf(_("%s: Wrong type for input argument #%d: Scalar or string expected.\n"),..
                     "damp",2))
  end
  if argn(2)==1 then
    if dt1<>[] then 
      dt=dt1
    else
      dt='c' //continuous time interpretation assumed
    end
  else
    if dt1<>[] then error();end
  end
  
  if type(dt)==1 then
    if size(dt,'*')<>1|dt<0 then
      error(msprintf(_("%s: Wrong type for input argument #%d: Real non negative scalar expected.\n"),..
                     "damp",2))
    end
  elseif type(dt)==10 then
    if dt=="c" then
      dt=0
    elseif dt=="d" then
      dt=1
    else
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "damp",2,"""c"", ""d"""))
    end
  else
    error(msprintf(_("%s: Wrong type for input argument #%d: Scalar or string expected.\n"),..
                   "damp",2))
  end
  im=ieee();ieee(2);//to allow inf and nan's
  if dt>0 then 
    // Discrete case, compute continuous time equivalent poles
    R=log(R)/dt;
  end
  wn = abs(R);
  z = -real(R)./wn;
  z(wn==0)=-1
  ieee(im)
  //order according to increasing natural pulsations
  if order then
    [wn,k]=gsort(wn,'g','i');
    z=z(k);
  end
endfunction


