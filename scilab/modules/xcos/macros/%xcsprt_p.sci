// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - INRIA -Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function %xcsprt_p(ports)
    //Display the cpr port field value
    F=fieldnames(ports)';
    labels=[_("port numbers:");_("block numbers:")];
    txt=[];
    for f=F
        p=ports(f)
        if ~isempty(p) then
            w=[labels,string(p')];
            t=emptystr(2,1);
            for k=1:size(w,2)
                t=t+part(w(:,k),1:max(length(w(:,k)))+2);
            end
            txt=[txt;
            "Ports "+f
            "    "+t];
        end
    end
    mprintf("  %s\n",txt);
endfunction
