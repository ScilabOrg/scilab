// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Sylvain Genin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function R=%r_f_r(r1,r2)

    [n1,m1] = size(r1);
    [n2,m2] = size(r2);
    if m1==m2 then
        R  = zeros(n1+n2,m1);
        den = [r1.den ; r2.den];
        num = [r1.num ; r2.num];
        R(1) = syslin('c',r1.num(1),r1.den(1));
        R(2) = num;
        R(3) = den;
    else
        R = error(msprintf(_("%s: Inconsistent dimensions.\n"),"%r_f_f"));
    end
endfunction
