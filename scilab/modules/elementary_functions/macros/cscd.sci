// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function x = cscd(x)
//Cosecant argument in degree
  if ~isreal(x)
    error(msprintf(_('%s :Argument should be real.'),'cscd'));
  end
  n = round(x/90);
  x = x - n*90;
  m = pmodulo(n,4);
  x(m==0) = 1 ./ sin(%pi/180*x(m==0));
  x(m==1) = 1 ./ cos(%pi/180*x(m==1));
  x(m==2) = -1 ./ sin(%pi/180*x(m==2));
  x(m==3) = -1 ./ cos(%pi/180*x(m==3));
endfunction
