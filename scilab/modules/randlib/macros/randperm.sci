// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2013 - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function v = randperm(n, k);

    rhs  = argn(2);

    if (rhs == 1 | rhs == 2) then
        if type(n) <> 1 then
            error(msprintf(_("%s: Wrong type for input argument #%d: An integer expected.\n"),"randperm",1));
        else
            if size(n, "*") <> 1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: (%d,%d) expected.\n"),"randperm",1,1,1));
            else
                if floor(n) <> n then
                    error(msprintf(_("%s: Wrong value for input argument #%d: An integer expected.\n"),"randperm",1));
                else
                    v = grand(1, "prm", (1:n)')';
                    if rhs == 1 then
                        return
                    else
                        if type(k) <> 1 then
                            error(msprintf(_("%s: Wrong type for input argument #%d: An integer expected.\n"),"randperm",2));
                        else
                            if size(k, "*") <> 1 then
                                error(msprintf(_("%s: Wrong size for input argument #%d: (%d,%d) expected.\n"),"randperm",2,1,1));
                            else
                                if floor(k) <> k then
                                    error(msprintf(_("%s: Wrong value for input argument #%d: An integer expected.\n"),"randperm",2));
                                else
                                    if n < k then
                                        error(msprintf(_("%s: Wrong value of input argument #%d: expected inferior to argument #%d.\n"),"randperm",2,1));
                                    else
                                        v = v(1:k);
                                        return
                                    end
                                end
                            end
                        end
                    end
                end
            end
        end
    else
        error(msprintf(_("Wrong number of input arguments.\n")));
    end
endfunction
