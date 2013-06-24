// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009,2013 - Université du Maine - Samuel Gougeon
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [nb, loc] = members(A, S, last)
    // [nb, loc] = members(A, S, last)
    //
    // A : Matrix of booleans, integers, reals, complexes, polynomials or strings:
    //      entities to search in S.
    // S : Matrix of same datatype as A.
    // last: Scalar boolean
    // nb  : Matrix of reals: same size as A
    //       nb(i,j,..): number of occurrences of A(i,j,..) in S.
    // loc : Matrix of reals: same size as A
    //       loc(i,j,..): linear index in S of the first occurrence of A(i,j,..)
    //       if last==%t, the index of the last occurrence is returned instead.
    //       loc(i,j,..) returns zero if A(i,j,..) is not found.
    //
    // %inf | -%inf values are supported either in A or S.

    [lhs, rhs] = argn();
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): at least %d expected.\n"), "members", 2));
    end
    if rhs==2 then
        last = %f
    end
    if A == [] then
        nb = [];
        if lhs>1 then
            loc = []
        end
        return
    end
    if  S == []  then
        nb = zeros(A)
        if lhs>1 then
            loc = zeros(A)
        end
        return
    end

    type_A = type(A);
    type_S = type(S);
    if type_A <> type_S then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: expected same type as first argument.\n"), "members", 2));
    end
    if type_A <> 1 & type_A <> 2 & type_A <> 4 & type_A <> 8 & type_A <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of Integers, Reals, Complexes, Booleans, Polynomials or Strings expected.\n"), "members", 1));
    end
    if type_S <> 1 & type_S <> 2 & type_S <> 4 & type_S <> 8 & type_S <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of Integers, Reals, Complexes, Booleans, Polynomials or Strings expected.\n"), "members", 2));
    end
    if type(last) <> 4 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean matrix expected.\n"), "members", 3));
    end
    if last <> %t & last <> %f then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: %s or %s expected.\n"), "members", 3, "%t", "%f"));
    end


    LA = size(A,"*");
    LS = size(S,"*");
    if type_A <> 10 then
        A2 = A(:)*ones(1, LS);
        S2 = (S(:)*ones(1, LA)).';
    else    // Strings => explicit replication (* unapplicable)
        A2 = emptystr(LA, LS);
        for i = 1:LS
            A2(:, i) = A(:);
        end
        S = S(:)';
        S2 = emptystr(LA, LS);
        for i=1:LA
            S2(i, :) = S;
        end
    end
    tmp = bool2s(A2==S2);
    clear A2 S2
    nb = matrix(sum(tmp,"c"), size(A))
    if lhs>1 then
        tmp = tmp.*( ones(LA, 1)*(1:LS) )
        if last == %t then
            tmp2 = max(tmp, "c")
        else
            tmp(tmp==0) = 2*LS;    // Masks zeros = unfound as min values
            tmp2 = min(tmp, "c");
            tmp2(tmp2==2*LS) = 0;  // Retrieves zeros = unfound
        end
        loc = matrix(tmp2, size(A));
    end

endfunction
