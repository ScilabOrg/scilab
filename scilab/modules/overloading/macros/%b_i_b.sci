// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function M=%b_i_b(varargin)
    //insertion of a boolean matrix in an matrix of boolean for more than 2 indices

    rhs=argn(2)
    M=varargin(rhs)
    M=mlist(["hm","dims","entries"],int32(size(M)),M(:))
    varargin(rhs)=M;
    M=generic_i_hm(%f,varargin(:))
endfunction
