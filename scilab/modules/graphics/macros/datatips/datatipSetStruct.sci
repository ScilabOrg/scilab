// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function ok=datatipSetStruct(curve,t)
    if argn(2)<>2 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),...
        "datatipSetStruct",2))
    end
    if type(curve)<>9|size(curve,"*")<>1|or(curve.type<>"Polyline") then
        error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
        "datatipSetType",1,"Polyline"))
    end

    ok=%t
    %datatips_i_s=generic_i_s
    %datatips_i_st=generic_i_st
    u_d=get(curve,"user_data")
    if u_d==[] then
        u_d.datatips=t
    else
        if typeof(u_d)=="st"  then
            u_d.datatips=t
        elseif typeof(u_d)=="datatips" //for compatibilty
            u.datatips=u_d
            u_d=u
        else
            ok=%f
            warning(msprintf(_("%s: Wrong type for curve user_data field: a struct expected.\n"),...
            "datatipSetStruct"))
        end
    end
    set(curve,"user_data",u_d)
endfunction
