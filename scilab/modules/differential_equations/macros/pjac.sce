function [wp, iwp] = pjac2(res, neq, t, y, ydot, h, cj, rewt, savr)

    i = 0;
    j = 0;
    nrow = 0;
    tx = 0;
    del = 0;
    delinv = 0;
    ysave = 0;
    ypsave = 0;
    e = NULL;
    iFour = 4;
    ires = 0;

    SQuround = 1.490D-08;

    tx = t;
    e = zeros(1, neq);

    wp = zeros(1, neq*neq);
    iwp = zeros(1, neq*neq);

    for i=1:neq
        del = max(SQuround*max(abs(y(i)),abs(h*ydot(i))), 1/rewt(i))
        if h*ydot(i) < 0 then del = -del; end
        ysave = y(i);
        ypsave = ydot(i);
        y(i) = y(i) + del;
        ydot(i) = ydot(i) + cj*del;
        res(tx, y, ydot, e, ires, 0, 0);
        if (ires < 0) return;
            delinv = 1/del;
            for j=1:neq
                wp(nrow+j) = delinv*(e(j)-savr(j));
            end
            nrow = nrow + neq;
            y(i) = ysave;
            ydot(i) = ypsave;
            iwp(i) = i-1;
            iwp(i+neq) = i-1;
        end

    endfunction
