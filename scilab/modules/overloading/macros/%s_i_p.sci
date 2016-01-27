// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function M=%s_i_p(varargin)
    //insertion of an polynomial matrix in an matrix of numbers for more than 2 indices
    M=varargin($)
    M=mlist(["hm","dims","entries"],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm(0,varargin(:))
endfunction
