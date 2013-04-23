// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [wp, iwp, ires] = pjac2(neq, t, y, ydot, h, cj, rewt, savr)

    ires = 0;
    SQuround = 1.490D-08;
    tx = t;
    nrow = 0;
    e = zeros(1, neq);

    wp = zeros(neq*neq, 1);
    iwp = zeros(neq*neq, 2);

    for i=1:neq
        del = max(SQuround*max(abs(y(i)), abs(h*ydot(i))), 1/rewt(i))
        if h*ydot(i) < 0 then del = -del; end
        ysave = y(i);
        ypsave = ydot(i);
        y(i) = y(i) + del;
        ydot(i) = ydot(i) + cj*del;
        [e ires]=res1(tx, y, ydot);
        if ires < 0 then return; end
        delinv = 1/del;
        for j=1:neq
            wp(nrow+j) = delinv*(e(j)-savr(j));
            iwp(nrow+j,1) = i;
            iwp(nrow+j,2) = j;
        end
        nrow = nrow + neq;
        y(i) = ysave;
        ydot(i) = ypsave;
    end

endfunction
