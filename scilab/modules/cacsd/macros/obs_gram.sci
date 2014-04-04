// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [go]=obs_gram(a,c,domaine)

    [lhs,rhs]=argn(0)
    select typeof(a)
    case "constant" then
        if rhs<2 then error(39); end;
        if rhs==2 then domaine="c"; end;
        if and(domaine<>["d","c"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "obs_gram",3,"''d'', ''c''"));
        end
        [m,n]=size(a)
        if m<>n then error(20,1),end
        [mb,nb]=size(c);if nb<>n then error(60),end
    case "state-space" then
        if rhs>1 then
            error(msprintf(gettext("%s: Wrong number of input arguments: %d expected"),"obs_gram",1)),
        end
        [a,c,domaine]=a([2,4,7])
        if domaine==[] then
            warning(msprintf(gettext("%s: Input argument #%d is assumed continuous time.\n"),"obs_gram",1));
            domaine="c";
        elseif type(domaine)==1 then
            domaine="d",
        end
        [n,n]=size(a)
    case "rational" then
        if rhs>1 then
            error(msprintf(gettext("%s: Wrong number of input arguments: %d expected"),"obs_gram",1)),
        end
        a=tf2ss(a)
        [a,c,domaine]=a([2,4,7])
        if domaine==[] then
            warning(msprintf(gettext("%s: Input argument #%d is assumed continuous time.\n"),"obs_gram",1));
            domaine="c";
        elseif type(domaine)==1 then
            domaine="d",
        end
        [n,n]=size(a)
    else
        if rhs==1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear dynamical system  expected.\n"),"obs_gram",1))
        else
            error(msprintf(gettext("%s: Wrong type of input argument #%d: Array of floating point numbers expected.\n"),"obs_gram",1))
        end
    end;
    //
    s=spec(a)
    if (domaine=="c"&max(real(s))>=0)|(domaine=="d"&max(abs(s))>=1) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Stable system expected.\n"),"obs_gram",1));
    end
    go=lyap(a,-c'*c,domaine)
endfunction
