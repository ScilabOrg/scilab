// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y = %sp_grand(n, prm, vect)
    // Overload to use grand on sparse input

    [m, n2] = size(vect);
    if n2 > 1 then
        error(msprintf(_("%s: Wrong type for input argument: Column vector expected.\n"), "grand"));
    end
    perm = grand(n, "prm", (1:m)'); // Generate the permutation indexes matrix

    y = perm; // Give the proper size and type to y
    for j=1:n
        for i=1:m
            y(i, j) = vect(perm(i, j)); // Proceed to permuation by index switching
        end
    end

    y = sparse(y);

endfunction
