// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Université du Maine - Samuel Gougeon
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function loc = ismember(A, S, highest)

    // loc = ismember(A, S, highest)
    //
    // A : Matrix of booleans, integers, reals, complexes, polynomials or strings
    // S : Matrix of same datatype than A
    // highest : Scalar boolean
    //
    // ismember() returns a matrix loc of A's format. loc(i, j) = linear
    //  index in S of the first (highest == %F), or the last (highest == %T)
    //  occurrence of A(i, j). loc(i, j) returns zero if A(i, j) is not found.
    //  %inf | -%inf values are supported either in A or S.
    //
    // COPYRIGHT : Samuel GOUGEON  - Université du Maine, France (2009)
    // LICENSE : CeCILL
    //

    [lhs, rhs] = argn();

    if rhs < 3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "ismember", 3));
    end

    type_A = type(A);
    type_S = type(S);
    if type_A <> type_S then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: expected same type as first argument.\n"), "ismember", 2));
    end
    if type_A <> 1 & type_A <> 2 & type_A <> 4 & type_A <> 8 & type_A <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of Integers, Reals, Complexes, Booleans, Polynomials or Strings expected.\n"), "ismember", 1));
    end
    if type_S <> 1 & type_S <> 2 & type_S <> 4 & type_S <> 8 & type_S <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of Integers, Reals, Complexes, Booleans, Polynomials or Strings expected.\n"), "ismember", 2));
    end
    if type(highest) <> 4 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean matrix expected.\n"), "ismember", 3));
    end
    if highest <> %t & highest <> %f then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: %s or %s expected.\n"), "ismember", 3, "%t", "%f"));
    end

    if type_A <> 10 then
        if A == [] | S == [] then
            loc = [];
            return;
        end
    end

    if type_A <> 10 then // Strings should be handled differently, because they don't support real multiplication
        LA = length(A);
        LS = length(S);
        A2 = matrix(A, LA, 1)*ones(1, LS);
        S2 = (matrix(S, LS, 1)*ones(1, LA)).';
        tmp = double(A2==S2);
        clear A2, S2;
        tmp = tmp.*( ones(LA, 1)*(1:LS) );
        if highest == %t then
            tmp2 = max(tmp, "c");
        else
            tmp(tmp==0) = 2*LS;    // Masks zeros = unfound as min values
            tmp2 = min(tmp, "c");
            tmp2(tmp2==2*LS) = 0;  // Retrieves zeros = unfound
        end
        loc = matrix(tmp2, size(A));
    else
        LA = size(A, "*");
        LS = size(S, "*");
        A_vec = matrix(A, LA, 1);
        A2 = emptystr(LA, LS);
        for i=1:LS
            A2(:, i) = A_vec;
        end
        S_vec = matrix(S, 1, LS);
        S2 = emptystr(LA, LS);
        for i=1:LA
            S2(i, :) = S_vec;
        end
        tmp = double(A2==S2);
        clear A2, S2;
        tmp = tmp.*( ones(LA, 1)*(1:LS) );
        if highest == %t then
            tmp2 = max(tmp, "c");
        else
            tmp(tmp==0) = 2*LS;    // Masks zeros = unfound as min values
            tmp2 = min(tmp, "c");
            tmp2(tmp2==2*LS) = 0;  // Retrieves zeros = unfound
        end
        loc = matrix(tmp2, size(A));
    end

endfunction
