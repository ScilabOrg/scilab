// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%b_prod(varargin)
    if varargin($)=="native" then
        if size(varargin)==3&type(varargin(2))==1&varargin(2)>2 then
            r=varargin(1)
        else
            r=and(varargin(1:$-1))
        end
    else
        varargin(1)=bool2s(varargin(1))
        r=prod(varargin(:))
    end
endfunction
