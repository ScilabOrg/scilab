// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2006 - Anthony Roberts <anthony.roberts@adelaide.edu.au>
// Copyright (C) Scilab Enterprises - 2014 - Pierre-Aime Agnel
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// =============================================================================

function [fvals] = contour3d(x, y, z, f, nf, varargin)
    // Draws 3D contours, iso-surfaces, of function f on
    // 3D lattice x, y, z.
    // DESCRIPTION contour3d  draws level surfaces of a
    // 3D function f(x, y, z) on a 3D plot.  The values of
    // f(x, y, z) are to be in the 3D hypermatrix f at the
    // lattice grid points defined by 1D matrices x, y and z.
    //
    // INPUT  If no arguments are supplied at all, then
    // contour3d draws a demonstration of nested spheres.
    // x, y, z: 1D matrices of coordinates of grid
    // points in space at which the function f is known.
    // f: 3D matrix of function values. Set any missing
    // f-values to %nan.
    // nf: if nf is simply a number of surfaces to draw,
    // then a colorbar is also drawn.  However, if nf is
    // a vector of surface values, then no colorbar. You
    // can skip one or more colors in the current
    // colormap by specifying %nan at the corresponding
    // place in the vector nf of f values---useful for
    // superimposing iso-surface plots.
    //
    // Any extra arguments to contour3d are passed on to
    // plot3d to empower you to set view, legend, and so on.
    //
    // OUTPUT fvals: vector of set contour values.
    //
    // Uses 40.nx.ny.nz memory or thereabouts.
    // Tony Roberts, 26 Aug 2006; modified Mar 2010.
    [nlhs, nrhs] = argn()

    // No input arguments: set example
    if nrhs == 0
        nx = 3; ny = 4; nz = 5;
        x = linspace(0, 1, nx);
        y = linspace(0, 1, ny);
        z = linspace(0, 1, nz);
        // make row/column/plane of hypermat
        xx = matrix(x, [nx, 1, 1]);
        yy = matrix(y, [1, ny, 1]);
        zz = matrix(z, [1, 1, nz]);
        xx = xx(:, ones(1, ny), ones(1, nz));
        yy = yy(ones(1, nx), :, ones(1, nz));
        zz = zz(ones(1, nx), ones(1, ny), :);
        // compute function and pass on to draw
        f = sqrt((xx-1).^ 2 + yy.^2 + zz.^2) - 0.5;
        nf = 6;
        varargin = list();
        clf()
        xset("colormap", hotcolormap(nf))
        //fs = contour3d(x, y, z, f, nf)
    end

    // checking input arguments

    // x, y, z and nf must be real values
    if type(x) ~= 1 // constant
        error(msprintf(_("%s: Wrong type for input arguments #%d: Real vector expected"), "contour3d", 1));
    end
    if type(y) ~= 1 //constant
        error(msprintf(_("%s: Wrong type for input arguments #%d: Real vector expected"), "contour3d", 2));
    end
    if type(z) ~= 1 // constant
        error(msprintf(_("%s: Wrong type for input arguments #%d: Real vector expected"), "contour3d", 3));
    end
    if type(nf) ~= 1 // constant
        error(msprintf(_("%s: Wrong type for input arguments #%d: Real vector expected"), "contour3d", 5));
    end

    if ~isreal(x) // Real values
        error(msprintf(_("%s: Wrong value for input arguments #%d: Real vector expected"), "contour3d", 1));
    end
    if ~isreal(y) // Real values
        error(msprintf(_("%s: Wrong value for input arguments #%d: Real vector expected"), "contour3d", 2));
    end
    if ~isreal(z) // Real values
        error(msprintf(_("%s: Wrong value for input arguments #%d: Real vector expected"), "contour3d", 3));
    end
    if ~isreal(f) // Real values
        error(msprintf(_("%s: Wrong value for input arguments #%d: Real matrix expected"), "contour3d", 4));
    end
    if ~isreal(nf) // Real values
        error(msprintf(_("%s: Wrong value for input arguments #%d: Real vector expected"), "contour3d", 5));
    end

    // x, y, z must be vectors
    if ~isvector(x)
        error(msprintf(_("%s: Wrong size for input arguments #%d: Vector expected"), "contour3d", 1));
    end
    if ~isvector(y)
        error(msprintf(_("%s: Wrong size for input arguments #%d: Vector expected"), "contour3d", 2));
    end
    if ~isvector(z)
        error(msprintf(_("%s: Wrong size for input arguments #%d: Vector expected"), "contour3d", 3));
    end

    len_x = length(x);
    len_y = length(y);
    len_z = length(z);

    // first get & check sizes of 3D arrays
    if length(size(f)) ~= 3
        error(msprintf(_("%s: wrong size for input arguments #%d: a %d-by-%d-by-%d matrix expected"), "contour3d", 3, len_x, len_y, len_z));
    end

    [nx, ny, nz] = size(f);

    if or([len_x, len_y, len_z] ~= [nx, ny, nz])
        error(msprintf(_("%s: Wrong size for input arguments #%d: a %d-by-%d-by-%d matrix expected"), "contour3d", 3, len_x, len_y, len_z));
    end
    x = matrix(x, [nx, 1, 1]);
    y = matrix(y, [1, ny, 1]);
    z = matrix(z, [1, 1, nz]);
    x = x(:, ones(1, ny), ones(1, nz));
    y = y(ones(1, nx), :, ones(1, nz));
    z = z(ones(1, nx), ones(1, ny), :);

    // find iso-surface values to include zero
    if (length(nf) == 1) & (nf == round(nf)) & (nf > 0)
        makefvals = %T;
        fvals = linspace(min(f), max(f), nf+2);
        maxf= fvals($);
        minf = fvals(1);
        fvals = fvals(2:$-1);
    else // use contour values in nf
        makefvals = %F;
        fvals = nf;
        maxf = max(nf);
        minf = min(nf);
        df = (maxf - minf) / (length(nf) - 1 + %eps);
        maxf = maxf + df/2;
        minf = minf - df/2;
    end

    // make a huge list of tetrahedrons
    // use these to cater for 8 different orientations of the 5 tetras
    i = 2 * ceil((1 : nx-1) / 2);
    id = (-1) .^ (1 : nx-1);

    j = 2 * ceil((1 : ny-1) / 2);
    jd = (-1) .^ (1 : ny-1);

    k = 2 * ceil((1 : nz-1) / 2);
    kd = (-1) .^ (1 : nz-1);

    l = (nx-1) * (ny-1) * (nz-1); // number of 3D bricks
    ntet = 5 * l;  // number of tetras

    // define tetras by their lattice point labels
    p = matrix([1 : nx * ny * nz], [nx, ny, nz]); // index of points
    ps = zeros(ntet, 4);

    ps(:, 1) = matrix([p(i, j, k); ..
    p(i+id, j, k+kd); ..
    p(i, j+jd, k+kd); ..
    p(i+id, j+jd, k); ..
    p(i+id, j, k)], ntet, 1);

    ps(:, 2) = matrix([p(i+id, j, k); ..
    p(i, j, k+kd); ..
    p(i, j, k+kd); ..
    p(i, j+jd, k); ..
    p(i, j+jd, k)], ntet, 1);

    ps(:, 3) = matrix([p(i, j+jd, k); ..
    p(i+id, j, k); ..
    p(i, j+jd, k); ..
    p(i+id, j, k); ..
    p(i, j, k+kd)], ntet, 1);

    ps(:, 4) = matrix([p(i, j, k+kd); ..
    p(i+id, j+jd, k+kd); ..
    p(i+id, j+jd, k+kd); ..
    p(i+id, j+jd, k+kd); ..
    p(i+id, j+jd, k+kd)], ntet, 1);

    fs = [f(ps(:, 1)) f(ps(:, 2)) f(ps(:, 3)) f(ps(:, 4))];

    // sort each tetra corner into decreasing order of f
    [fs, ll] = gsort(fs, "c");
    ps(:) = ps(sub2ind([ntet, 4], (1:ntet)' * ones(1, 4), ll));

    // only use tetras that have all vertices defined
    ok = find(~isnan(sum(fs, "c")));

    // build facets for each isosurface value
    xf = [];
    yf = [];
    zf = [];
    cf = [];
    for col = 1:length(fvals)
        fval = fvals(col);
        i = sum(fs(ok, :) > fval, "c"); // how many values > contour

        // do all the 1-3 tetras
        j = ok(find(i == 1));
        if ~isempty(j) then
            k = ones(1, 4);
            l = [2 2:4];
            rats = (fval - fs(j, l)) ./ (fs(j, k) - fs(j, l));
            xf = [xf; rats .* matrix(x(ps(j, k)), -1, 4) + (1-rats) .* matrix(x(ps(j, l)), -1, 4)];
            yf = [yf; rats .* matrix(y(ps(j, k)), -1, 4) + (1-rats) .* matrix(y(ps(j, l)), -1, 4)];
            zf = [zf; rats .* matrix(z(ps(j, k)), -1, 4) + (1-rats) .* matrix(z(ps(j, l)), -1, 4)];
            cf = [cf col(ones(j))];
        end

        // do all the 2-2 tetras
        j = ok(find(i == 2));
        if ~isempty(j) then
            k = [1 1 2 2];
            l = [3 4 4 3];
            rats = (fval - fs(j, l)) ./ (fs(j, k) - fs(j, l));
            xf = [xf; rats .* matrix(x(ps(j, k)), -1, 4) + (1-rats) .* matrix(x(ps(j, l)), -1, 4)];
            yf = [yf; rats .* matrix(y(ps(j, k)), -1, 4) + (1-rats) .* matrix(y(ps(j, l)), -1, 4)];
            zf = [zf; rats .* matrix(z(ps(j, k)), -1, 4) + (1-rats) .* matrix(z(ps(j, l)), -1, 4)];
            cf = [cf col(ones(j))];
        end

        // do all the 3-1 tetras
        j = ok(find(i == 3));
        if ~isempty(j) then
            k = [1 1:3];
            l = 4*ones(1, 4);
            rats = (fval - fs(j, l)) ./ (fs(j, k) - fs(j, l));
            xf = [xf; rats .* matrix(x(ps(j, k)), -1, 4) + (1-rats) .* matrix(x(ps(j, l)), -1, 4)];
            yf = [yf; rats .* matrix(y(ps(j, k)), -1, 4) + (1-rats) .* matrix(y(ps(j, l)), -1, 4)];
            zf = [zf; rats .* matrix(z(ps(j, k)), -1, 4) + (1-rats) .* matrix(z(ps(j, l)), -1, 4)];
            cf = [cf col(ones(j))];
        end
    end

    // draw a colorbar if not a vector of requested iso-surfaces
    if makefvals
        colorbar(minf, maxf, [1, size(fvals, "*") + 2])
    end

    // draw facets according to current colormap
    plot3d(xf', yf', list(zf',1 + cf), varargin(:))
    fog = gce();
    fog.hiddencolor = -1; // make front and back same color
    clear p ps xf yf zf x y z cf fs ll ok i j k l rats

endfunction
