// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - INRIA - Carlos Klimann
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function s=nansum(x,orient)
    //
    //This function returns in scalar or vector s the sum of the
    //values (ignoring the NANs) of a vector or matrix x.
    //
    //For a vector or matrix  x, s=nansum(x) returns in scalar s
    //the sum of all the entries (ignoring the NANs) of x.
    //
    //s=nansum(x,'r')(or,  equivalently,  s=nansum(x,1)) returns
    //in each  entry of the  row vector s of  type 1xsize(x,'c')
    //the sum of each column of x (ignoring the NANs).
    //
    //s=nansum(x,'c')(or,  equivalently,  s=nansum(x,2)) returns
    //in each entry of the column vector s of type size(x,'c')x1
    //the sum of each row of x (ignoring the NANs).
    //
    //
    //fixed: 2003/09/03
    //error texts and all NAN rows or columns
    //
    rhs = argn(2);
    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"),"nansum",1,2))
    end

    if or(type(x) == [10 15 16]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex, boolean, polynomial matrix expected.\n"), "nansum", 1));
    end

    if isempty(x) | and(isnan(x)) then
        s = 0;
        return
    end

    if rhs == 1 then
        orient = "*";
    else
        if and(type(orient) <> [1  10])  then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A string or scalar expected.\n"), "nansum", 2));
        end

        if ~or(orient == ["r", "*", "c"] | orient == [1, 2]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "nansum", 2, """r"", ""c"", ""*"", 1, 2"));
        end
    end

    isn=isnan(x)
    x(isn)=0
    s=sum(x,orient)
endfunction
