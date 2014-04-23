// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [y, z] = filter(b, a, x, z)

    //Implements a direct form II transposed implementation of the standard
    //difference equation
    if type(b) == 2 | type(a) == 2
        c = b/a;
        b = c.num;
        a = c.den;
        deg_b = degree(b);
        deg_a = degree(a);
        deg = max(deg_b, deg_a);
        b = coeff(b, deg:-1:0);
        a = coeff(a, deg:-1:0);
    end

    //remove high order coefficients equal to zero
    i = 0; while b($ - i) == 0, i = i + 1; end;
    b = b(1:$ - i);

    //remove high order coefficients equal to zero
    i = 1; while a(i) == 0, i = i + 1; end
    a = a(i:$);

    //force vector orientation
    b   = matrix(b, -1, 1);
    a   = matrix(a, -1, 1);
    mnx = size(x);
    x   = matrix(x, 1, -1);

    //normalize
    b = b / a(1);
    a = a / a(1);

    n = max(size(b, "*"), size(a, "*"))-1;
    if n > 0 then
        if argn(2) < 4 then
            z = zeros(n, 1);
        else
            z = matrix(z, n, 1);
        end

        //pad the numerator and denominator if necessary
        a($ + 1:(n + 1)) = 0;
        b($ + 1:(n + 1)) = 0;

        //the algorithm below can also be used but is slower if x is long
        //may be good to write it in C
        //  for i = 1:size(x, '*')
        //    y(i) = z(1) + b(1)*x(i);
        //    z(1:(n-1)) = z(2:n) - a(2:n)*y(i) + b(2:n)*x(i);
        //    z(n) = b($)*x(i) - a($) * y(i);
        //  end

        //form state space representation
        A     = [-a(2:$), [eye(n - 1, n - 1); zeros(1, n - 1)] ];
        B     = b(2:$) - a(2:$) * b(1); //C = eye(1, n); D = b(1);

        [z, X] = ltitr(A, B, x, z);
        y     = X(1, :) + b(1) * x;
        //y = C*X+D*x
    else
        y     = b(1) * x;
        z     = [];
    end
    //make y orientation similar to the x one
    y = matrix(y, mnx);

endfunction

