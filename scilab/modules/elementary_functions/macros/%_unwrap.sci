// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %_unwrap()
    // unwrap() examples for samples (and demo)
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
