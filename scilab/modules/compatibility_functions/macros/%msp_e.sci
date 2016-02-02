// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function w=%msp_e(varargin)
    z=sci_sparse(varargin($));
    if length(varargin)==2 then
        w=mtlb_sparse(z(varargin(1)));
        return
    end
    i=varargin(1);j=varargin(2);
    w=mtlb_sparse(z(i,j));
endfunction
