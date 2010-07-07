// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function a=%sp_sum(a,d,typ)
  rhs=argn(2)
  if rhs==1 then
    d='*'
  elseif rhs==2 then 
    if argn(2)==2& or(d==['native','double']) then
      d='*'
    end
  end
  if size(d,'*')<>1 then 
    if type(d)==10 then
      error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"sum",2))
    else
      error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"sum",2))
    end
  end
  dims=size(a)
  if type(d)==10 then
    d=find(d==['m','*','r','c'])
    if d==[] then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "sum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
    end
    d=d-2
  end
  
  if d==-1 then 
    //sum(x,'m'), determine the summation direction
    d=find(dims>1,1)
    if d==[] then d=0,end
  end
  if d<0 then
    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "sum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
  end
  select d
    case 0 then
    //sum of all elements
    a=sum(a)
  case 1 then
     a=sparse(ones(1,dims(1))*a);
  case 2 then
    a=sparse(a*ones(dims(2),1))
  else
    a=a
  end
endfunction
