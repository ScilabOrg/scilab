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
    // A   : Matrix or hypermatrix of booleans, integers, reals, complexes, polynomials or strings:
    //       entities (needles) to search in S (haystack).
    // S   : Matrix or hypermatrix of same datatype as A
    // last: Scalar boolean
    // nb  : Matrix of reals: same sizes as A
    //       nb(i, j, ..): number of occurrences of A(i, j, ..) in S.
    // loc : Matrix of reals: same sizes as A
    //       loc(i, j, ..): linear index in S of the first occurrence of A(i, j, ..).
    //       If last==%t, the index of the last occurrence is returned instead.
    //       loc(i, j, ..) returns zero if A(i, j, ..) is not found.
    //
    // %inf, -%inf values are supported in A as well as in S.
    // %nan are supported only in A.
    // Example:
    // N = [ 7  3
    //     %inf 0
    //     %nan 1 ];
    // H = [ 5   8    0   4
    //       3   4    7   7
    //       3 %inf %inf  2
    //       7   5    5   8 ];
    // [nb, loc] = members(N, H)
    // [nb, loc] = members(N, H, %T)

    [lhs, rhs] = argn();
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): at least %d expected.\n"), "members", 2));
    end
    if rhs == 2 then
        last = %f;
    end
    if A == [] then
        nb = [];
        if lhs > 1 then
            loc = [];
        end
        return
    end
    if  S == []  then
        nb = zeros(A);
        if lhs > 1 then
            loc = zeros(A);
        end
        return
    end

    type_A = type(A(:));
    type_S = type(S(:));
    if type_A <> type_S then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: expected same type as first argument.\n"), "members", 2));
    end
    if type_A <> 1 & type_A <> 2 & type_A <> 4 & type_A <> 8 & type_A <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of Integers, Reals, Complexes, Booleans, Polynomials or Strings expected.\n"), "members", 1));
    end
    if type_S <> 1 & type_S <> 2 & type_S <> 4 & type_S <> 8 & type_S <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of Integers, Reals, Complexes, Booleans, Polynomials or Strings expected.\n"), "members", 2));
    end
    if or(isnan(S)) then
        error(msprintf(gettext("%s: Wrong value for argument #%d: Must not contain NaN.\n"), "members", 2));
    end
    if type(last) <> 4 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean matrix expected.\n"), "members", 3));
    end
    if ~isscalar(last) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Scalar expected.\n"), "members", 3));
    end

    // Reals: special faster processing with dsearch().
    //  %inf, -%inf are supported in A and S. %nan are only supported in A.
    //  Processing integers and strings requires bugs 6305 & 12778 to be fixed.
    if type_A == 1 & isreal(A) & isreal(S) then
        S = S(:);
        if last then
            S = S($:-1:1);
        end
        [Su, kS] = unique(S);
        [i, nbS] = dsearch(S, Su, "d");

        I = dsearch(A(:), Su, "d");
        I = matrix(I, size(A));
        k = find(I~=0);
        nb = I;
        nb(k) = nbS(I(k));
        if lhs > 1 then
            loc = I;
            loc(k) = kS(I(k));
            if last then
                loc(k) = length(S)-loc(k)+1;
            end
        end

    else
        // All other cases
        LA = size(A, "*");
        LS = size(S, "*");
        if ~last then
            S = S($:-1:1);
        end
        if type_A <> 10 then
            A2 = A(:)*ones(1, LS);
            S2 = (S(:)*ones(1, LA)).';
        else    // Strings => explicit replication (* unapplicable)
            A2 = repmat(A(:), 1, LS);
            S2 = repmat(S(:).', LA, 1);
        end
        tmp = bool2s(A2==S2);
        clear A2 S2
        nb = matrix(sum(tmp, "c"), size(A));
        if lhs > 1 then
            tmp = tmp.*( ones(LA, 1)*(1:LS) );
            tmp2 = max(tmp, "c");
            if ~last
                k = tmp2~=0;
                tmp2(k) = LS - tmp2(k) + 1;
            end
            loc = matrix(tmp2, size(A));
        end
    end

endfunction
