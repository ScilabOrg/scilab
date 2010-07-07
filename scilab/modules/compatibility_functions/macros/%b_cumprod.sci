// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function a=%b_cumprod(varargin)
  if varargin($)=='native' then
    if size(varargin)==2 then
      d='*'
    elseif size(varargin)==3 then 
      d=varargin(2)
    end
    if size(d,'*')<>1 then 
      if type(d)==10 then
        error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumprod",2))
      else
        error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumprod",2))
      end
    end
    a=varargin(1)
    if type(d)==10 then
      d=find(d==['m','*','r','c'])
      if d==[] then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                       "cumprod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
      end
      d=d-2
    end
    dims=size(a);
    
    if d==-1 then //'m'
      d=find(dims>1,1)
      if d==[] then d=0,end
    end
    if d<0 then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "cumprod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
    end
    select d
    case 0 then
      k=find(~a,1)
      if k<>[] then a(k:$)=%f,end
    case 1 then
      for k=1:dims(2)
        a(:,k)=cumprod(a(:,k),'native')
      end
    case 2 then
      for k=1:dims(1)
        a(k,:)=cumprod(a(k,:),'native')
      end
    else
      a=a
    end    
  else
    varargin(1)=bool2s(varargin(1))
    a=cumprod(varargin(:))
  end
endfunction
