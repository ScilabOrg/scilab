// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//a=toeplitz(c,r)  returns the Toepliz matrix whose first row
//is r and first column is c .( r(1) = c(1) is assumed).
//
//r and  c can be constant, polynomial or character string matrices.
//!

function a=toeplitz(c,r)
  [lhs, rhs] = argn(0);
  if ~find(type(c) == [1,2,10]) then
     error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"), "toeplitz", 1));
  end
  a = [];
  if rhs == 1 then
    r = c(:);
  end
  nr = size(r, '*');
  r = matrix(r, 1, nr);
  nc = size(c, '*'); 
  c = matrix(c, nc, 1);
  if nr*nc == 0 then 
    a = [];
    return
  end
  if r(1) <> c(1) then
    error(msprintf(gettext("%s: Wrong values for input arguments #%d and #%d: c(1) must be equal to r(1).\n"), "toeplitz", 1, 2));
  end
  // initialize a  
  k = min([nr, nc]);
  a = zeros(k, k);
  if type(c) == 10 then
    a = string(a);
  end
  
  a(nc, nr) = r(1);
  
  for l = 1:k
    a(l, l:nr) = r(1:nr-l+1);
    a(l:nc, l) = c(1:nc-l+1);
  end
endfunction
