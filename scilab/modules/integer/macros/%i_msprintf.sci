// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function r=%i_msprintf(fmt,varargin);
    for k=1:size(varargin)
        if type(varargin(k))==8 then
            varargin(k)=double(varargin(k))
        end
    end
    r=msprintf(fmt,varargin(:))
endfunction
