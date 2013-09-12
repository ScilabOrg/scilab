// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function r=%s_b_s(varargin)
    //a:b:c where a and b are double
    if round(varargin(1))<>varargin(1)|round(varargin(2))<>varargin(2) then
        error(msprintf(_("%s: Wrong type for input arguments #%d and #%d: Integer values expected.\n"),"%s_b_s",1,2));
    end
    select type(varargin(3))
    case 8 then
        it=inttype(varargin(3))
        r=iconvert(varargin(1),it):iconvert(varargin(2),it):varargin(3)
    else
        error(msprintf(_("%s: Not yet implemented.\n"),"%s_b_s"));
    end
endfunction


