// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000-2011 - INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%i_e(varargin)
//M(i,j,k,..)
  nind=size(varargin)-1;
  M=varargin($);
  if size(M,'*')==1 then //M(I), M scalar and I hypermatrix
    f=%hm_e(varargin(:));
  else //M 2D array with more than 2 indexes

    //here whe do not use hypermat because it removes highest dimensions
    //equal to 1
    M=mlist(['hm','dims','entries'],int32([size(M),ones(1,nind-2)]),M(:));
    f=M(varargin(1:$-1))
  end
endfunction
