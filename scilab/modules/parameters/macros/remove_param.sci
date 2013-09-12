// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO 2008-2010 - Yann COLLETTE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [plist,err] = remove_param(list_name,param_name)
    [nargout,nargin] = argn();
    plist = [];
    if typeof(list_name)=="plist" then
        if is_param(list_name,param_name) then
            Index = grep(getfield(1,list_name),param_name);
            Aux = getfield(1,list_name);
            if Index==1 then
                Aux = Aux(:,2:$);
            elseif Index==size(getfield(1,list_name),2) then
                Aux = Aux(:,1:$-1);
            else
                Aux = Aux(:,[1:Index-1 Index+1:size(Aux,2)]);
            end
            new_list = mlist(Aux);
            New_Index = 2;
            for i=2:size(getfield(1,list_name),2)
                if i==Index then continue; end
                new_list(Aux(New_Index)) = getfield(i,list_name);
                New_Index = New_Index + 1;
            end
        else
            plist = list_name;
        end
        plist = new_list;
        if nargout==2 then err = %F; end
    else
        if nargout==2 then
            err = %T;
        else
            error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "remove_param", 1, "plist"));
        end
    end
endfunction
