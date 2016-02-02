// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function %i_mfprintf(id, fmt, varargin);

    if type(id) == 8 then
        id = int(id);
    end

    for k=1:size(varargin)
        if type(varargin(k)) == 8 then
            varargin(k) = double(varargin(k))
        end
    end

    mfprintf(id, fmt, varargin(:));

endfunction
