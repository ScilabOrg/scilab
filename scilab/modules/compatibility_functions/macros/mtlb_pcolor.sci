// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function h = mtlb_pcolor(varargin)

  f = gcf();
  h = f.figure_id;

  //find first propertyname position
  rhs = size(varargin);
  for k=1:rhs
    if type(varargin(k))==10 then
      rhs = k-1;
      break;
    end
  end

  C = varargin(rhs)';

  if rhs==3 then
    grayplot(varargin(1:2),C)
  else
    grayplot(1:size(C,1),1:size(C,2),C)
  end

endfunction
