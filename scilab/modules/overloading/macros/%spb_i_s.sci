// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M=%spb_i_s(varargin)
// %sp_i_s(i,j,b,a) insert sparse matrix b into full matrix a
// M(i,j)=b
//!

  [lhs,rhs]=argn(0)

  M=varargin(rhs)
  N=varargin(rhs-1)//inserted matrix
  index=varargin(1) //

  if rhs==3&(type(index)==10|type(index)==15) then
    M=createstruct(index,N)
    if type(index(1))<>10 & index(2)=="entries" then
      // change struct to cell
      f=getfield(1,M);f(1)="ce"
      setfield(1,f,M)
    end
  else
    M(varargin(1:rhs-2))=full(N)
  end
endfunction
