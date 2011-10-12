// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function i = pmodulo(n, m)
  //i=pmodulo(n,m) the "positive modulo" of m et n.
  //i=n-floor(n./m).*m
  
  [lhs, rhs] = argn(0);
  if rhs <> 2 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "pmodulo", 2));
  end
  
  if size(n,'*') == 1 then
    i = zeros(m);
    k = find(m == 0)
    i(k) = n;
    k = find(m~=0);
    i(k) = n-floor(n./m(k)).*m(k);
  elseif size(m,'*') == 1 then
    i = zeros(n);
    if m == 0 then
      i = n;
    else
      i = n-floor(n./m).*m;
    end
  else
    if or(size(n)<>size(m)) then 
      error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"pmodulo"));
    end
    i = zeros(n);
    k = find(m==0);
    i(k) = n(k);
    k = find(m~=0);
    i(k) = n(k)-floor(n(k)./m(k)).*m(k);
  end
endfunction
