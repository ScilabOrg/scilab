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
        end
        nrow = nrow + neq;
        y(i) = ysave;
        ydot(i) = ypsave;
        iwp(i,1) = i-1;
        iwp(i,2) = i-1;
    end
endfunction
