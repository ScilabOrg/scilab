//------------------------------------------------------------------------------------------------------------
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// Returns the last day of the year and month by corresponding element of Matrix Y and M
//------------------------------------------------------------------------------------------------------------

function E=eomday(Y,M)

    rhs=argn(2);

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"eomday", 2));
    end

    common_year = [31,28,31,30,31,30,31,31,30,31,30,31];
    leap_year   = [31,29,31,30,31,30,31,31,30,31,30,31];

    if type(Y) <> 1 | ~isreal(Y) | int(Y) <> Y then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer value expected.\n"), "eomday", 1));
    end

    if type(M) <> 1 | ~isreal(M) | int(M) <> M then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer value expected.\n"), "eomday", 2))
    end

    if or(size(Y) <> size(M)) then
        error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: Same sizes expected.\n"),"eomday", 1, 2));
    end

    if (min(M) < 1) | (max(M) > 12) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"eomday",2,1,12));
    end

    [nr,nc] = size(M);
    E = zeros(1, nr*nc);
    isleapyear = isLeapYear(Y);
    E(isleapyear) = leap_year(M(isleapyear));
    E(~isleapyear) = common_year(M(~isleapyear));
    E = matrix(E,nr,nc);

endfunction
