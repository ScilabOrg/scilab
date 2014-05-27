// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - INRIA
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2013 - Samuel GOUGEON : extension to any mlist and tlist
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// This function returns TRUE if "field" is a field of the struct "s" or of
//  the tlist or mlist "s", FALSE otherwise

function r = isfield(s,field)

    function res = isfield1617(s,field)
        res = [];

        [nr,nc]     = size(field);
        fields      = getfield(1,s);
        if  isstruct(s) then
            cancel = 1:2
        else
            cancel = 1
        end
        fields(cancel) = [];

        for row=1:nr
            for line=1:nc
                res(row,line) = or(fields==field(row,line));
            end
        end
    endfunction

    r = [];
    rhs = argn(2);

    if rhs <> 2 then
        msg = _("%s: Wrong number of input argument(s): %d expected.\n");
        error(msprintf(msg,"isfield",2));
    end

    if ~or(type(s)==[15 16 17]) then
        msg = _("%s: Wrong type for input argument #%d: struct array, tlist, mlist or list of struct array or tlist or mlist expected.\n");
        error(msprintf(msg,"isfield",1));
    end

    if type(field) <> 10 then
        msg = _("%s: Wrong type for input argument #%d: A string expected.\n");
        error(msprintf(msg,"isfield",2));
    end

    if type(s) == 15 then
        if size(field, "*") <> 1 then
            msg = _("%s: Wrong size for input argument #%d: Single string expected.\n");
            error(msprintf(msg,"isfield",2));
        end
        for i_s = 1:size(s)
            if ~or(type(s(i_s))==[16 17]) then
                msg = _("%s: Wrong type for input argument #%d: struct array, tlist, mlist or list of struct array or tlist or mlist expected.\n");
                error(msprintf(msg,"isfield",1));
            else
                r = [r; isfield1617(s(i_s),field)];
            end
        end
    else
        r = isfield1617(s,field);
    end

endfunction
