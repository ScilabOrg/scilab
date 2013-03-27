// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Samuel GOUGEON
// Copyright (C) - 2013 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function retval = unwrap(a, tol, dim)

    [lhs, rhs]=argn(0);

    if (rhs <> 3) then
        if (rhs == 2) then
            if (size(a, "r") == 1) then
                dim = 1;
            else
                dim = 2;
            end
        elseif (rhs == 1) then
            tol = %pi;
            if (size(a, "r") == 1) then
                dim = 1;
            else
                dim = 2;
            end
        elseif (rhs == 0) then
            unwrap_demo();
            retval = [];
            return
        else
            error(msprintf(gettext("%s: Wrong number of input argument(s): at maximum %d expected.\n"), "unwrap", 3));
        end
    end

    if (tol == []) then
        tol = %pi;
    end
    tol = abs(tol);

    rng = 2*%pi;

    if (dim == 1)
        ra = a.';
    else
        ra = a;
    end
    [m,n] = size(ra);

    if (m == 1)
        retval = a;
        return;
    end

    d = [zeros(1, n); ra(1:m-1,:)-ra(2:m,:)];
    d(abs(d)<=tol) = 0
    k = (d~=0)
    d(k) = sign(d(k)).*rng

    r = cumsum(d);

    if (dim == 1) then
        retval = (ra + r).';
    else
        retval = (ra + r);
    end

endfunction

function unwrap_demo()

    // Display a full example
    drawlater()
    // Example 2.a in 1D:
    n = 400;
    tmax = 9.8;
    t = linspace(-tmax/2, tmax/2, n);
    y = t.^2;
    m = pmodulo(y,6);
    jump_max_in_data = max(abs(m(1:$-1)-m(2:$)))
    unwrap_jump = jump_max_in_data*0.97;
    u = unwrap(m, unwrap_jump);
    clf
    subplot(2,3,1)
    plot2d(t, y)
    title(_("Original profile Y"))
    subplot(2,3,2)
    plot2d(t, m)
    title("Y modulo 6")
    subplot(2,3,3)
    plot2d(t, u)
    title(sprintf(_("(Y modulo 6) unwrapped (jump_max=%4.2f)"),unwrap_jump))
    // EXAMPLE 2.b in 2D:
    n = 300;
    rmax = 9.8;
    r = linspace(-rmax/2, rmax/2, n);
    [x, y] = meshgrid(r, r);
    z = x.^2 + y.^2;
    m = pmodulo(z, 6);
    u = unwrap(m, 5);
    f = gcf();
    f.color_map = hotcolormap(100);
    subplot(2,3,4)
    surf(r,r,z)
    title(_("Original profile Z"))
    e = gce();
    e.thickness = 0; // removing the grid
    subplot(2,3,5)
    grayplot(r,r,m)
    title("Z modulo 6 (flat)")
    subplot(2,3,6)
    grayplot(r,r,u)
    title(_("(Z modulo 6) unwrapped (jump_max=5) (flat)"))
    drawnow()

endfunction
