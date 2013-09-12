// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [N,D]=lcmdiag(H,flag)
    //returns N and diagonal D
    //such that:
    //  flag='row' => H=D^(-1)*N  and D(k,k)=lcm of kth row of H('den')
    //  flag='col' => H=N*D^(-1)  and D(k,k)=lcm of kth col of H('den')
    // default flag = 'col'

    if (type(H)<>2 & type(H)<>16) then
        error(msprintf(_("%s: Wrong type for argument #%d: Real Matrix of Polynomials expected.\n"), "lcmdiag", 1));
    end

    Num = H("num");
    Den = H("den");
    [m, n] = size(H);
    D = []; N = [];
    [lhs, rhs] = argn(0);
    if rhs==1 then
        flag = "col";
    elseif type(flag)<>10 then
        error(msprintf(_("%s: Wrong type for argument #%d: String expected.\n"), "lcmdiag", 2));
    end
    select flag
    case "row"
        for k = 1:m
            [pgcd, fact] = lcm(Den(k, :));  //H(k,:)=(Num(k,:).*fact)/pgcd
            D = diag([diag(D); pgcd]);
            N = [N; Num(k, :).*fact];
        end
    case "col"
        for k = 1:n
            [pgcd, fact] = lcm(Den(:, k));
            D = diag([diag(D); pgcd]);
            N = [N, Num(:, k).*fact];
        end
    else
        error(msprintf(_("%s: Wrong value for argument #%d: %s or %s expected.\n"), "lcmdiag", 2, "row", "col"));
    end

endfunction
