// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2014 - Pierre-Aime Agnel
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// =============================================================================

function implicitplot3d(fun, x_range, y_range, z_range, varargin)

    [lhs, rhs] = argn(0);

    if rhs == 0 then
        scf();
        x_range = -1.1:0.1:1.1;
        y_range = x_range;
        z_range = x_range;
        implicitplot3d("x^2 + y^2 + z^2 - 1", x_range, y_range, z_range);
        return;
    end

    // x and y must be constants
    if rhs == 1 then
        x_range = linspace(-1, 1, 26);
        y_range = linspace(-1, 1, 26);
        z_range = linspace(-1, 1, 26);
        varargin = [];
    end
    if rhs == 2 then
        y_range = linspace(-%pi, %pi, 26);
        z_range = linspace(-%pi, %pi, 26);
        varargin = [];
    end
    if rhs == 3 then
        z_range = linspace(-%pi, %pi, 26);
        varargin = [];
    end

    if rhs >= 2 then
        if type(x_range) <> 1 then
            error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "implicitplot3d", 2));
        end
        if ~isvector(x_range) then
            error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "implicitplot3d", 2));
        end
        if ~isreal(x_range) then
            error(999, msprintf(gettext("%s: Wrong value for input argument #%d: Real value expected.\n"), "implicitplot3d", 2));
        end
    end

    if rhs >= 3 then
        if type(y_range) <> 1 then
            error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "implicitplot3d", 3));
        end
        if ~isvector(y_range) then
            error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "implicitplot3d", 3));
        end
        if ~isreal(y_range) then
            error(999, msprintf(gettext("%s: Wrong value for input argument #%d: Real value expected.\n"), "implicitplot3d", 3));
        end
    end

    if rhs >= 3 then
        if type(z_range) <> 1 then
            error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "implicitplot3d", 4));
        end
        if ~isvector(z_range) then
            error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "implicitplot3d", 4));
        end
        if ~isreal(z_range) then
            error(999, msprintf(gettext("%s: Wrong value for input argument #%d: Real value expected.\n"), "implicitplot3d", 4));
        end
    end

    // fun is a string or a function
    fun_type = type(fun);
    if ~or(fun_type == [17 10 11 13]) then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real hypermatrix or String or Scilab function expected.\n"), "implicitplot3d", 1));
    end

    if fun_type == 17 then
        if type(fun.entries) ~= 1 // fun is not of constant type
            error(999, msprintf(gettext("%s: Wrong type for input argument #%d: Real hypermatrix expected.\n"), "implicitplot3d", 1));
        end
        if ~isreal(fun.entries) // fun is not real
            error(999, msprintf(gettext("%s: Wrong value for input argument #%d: Real hypermatrix expected.\n"), "implicitplot3d", 1));
        end
        if or(size(fun) ~= [length(x_range), length(y_range), length(z_range)])
            error(999, msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d-by-%d matrix expected.\n"), "implicitplot3d", 1, length(x_range), length(y_range), length(z_range)));
        end
    else
        if fun_type == 10
            if size(fun, "*") ~= 1
                error(999, msprintf(gettext("%s: Wrong size for input argument #%d: Scalar string expected.\n"), "implicitplot3d", 1));
            end
        end
    end

    // Do not signal INF and NAN
    old_ieee = ieee();
    ieee(2);

    if type(fun) ~= 1
        // Define the function
        if type(fun) == 10 then
            deff("[t] = internal_fun(x, y, z)", ..
            "t = " + fun, "c");
        else
            internal_fun = fun;
            if type(fun) == 11 then
                comp(internal_fun);
            end
        end

        nx = length(x_range);
        ny = length(y_range);
        nz = length(z_range);
        t = zeros(nx, ny, nz)
        for k = 1:nz
            for j = 1:ny
                for i = 1:nx
                    t(i, j, k) = internal_fun(x_range(i), y_range(j), z_range(k));
                end
            end
        end
        t = matrix(t, nx, ny, nz);
        clear nx, ny, nz;

    else
        t = fun;
    end

    // chenck if zeros crossing happen
    if (min(t) * max(t) > 0)
        error(999, msprintf(gettext("%s: Function evaluation in selected range does not cross any zeros.\n"), "implicitplot3d"));
    end

    contour3d(x_range, y_range, z_range, t, [0, %nan], varargin);
    clear t;
    ieee(old_ieee);

endfunction
