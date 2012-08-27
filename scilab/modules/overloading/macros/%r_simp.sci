// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function h=%r_simp(h)
    // implement complex case
    num = h(2);
    den = h(3);
    tmp_h2 = 0;
    tmp_h3 = 0;
    var_name_num = varn(num);
    var_name_den = varn(den);

    if var_name_num <> var_name_den then
        error(msprintf(gettext("%s: Wrong value for input argument #%d and #%d: variable name of the numerator and the denominator must be the same.\n"), "simp", 1, 2));
    end

    var_name = poly(0,var_name_num);
    [m1,m2] = size(num);
    n = zeros(m1,m2);
    d = zeros(m1,m2);
    for k = 1:m1
        for l = 1:m2
            d_num = degree(num(k,l));
            d_den = degree(den(k,l));
            c_num = coeff(num(k,l));
            c_den = coeff(den(k,l));
            if d_num > d_den then
                deg = d_den;
            else
                deg = d_num;
            end

            if c_num(1) <> 0 | c_den(1) <> 0 then
                tmp_n = num(k,l);
                tmp_d = den(k,l);
                if tmp_n == tmp_d then
                    tmp_n = 1*var_name^0;
                    tmp_d = 1*var_name^0;
                end
            else
                for i = 2:deg + 1
                    if c_num(i) <> 0 | c_den(i) <> 0 then
                        deg = i-1;
                        break;
                    end
                end

                [nnr, ndr] = simp(real(num(k,l)), var_name^deg);
                [nni, ndi] = simp(imag(num(k,l)), var_name^deg);
                tmp_n = nnr + nni * %i;

                [dnr, ddr] = simp(real(den(k,l)), var_name^deg);
                [dni, ddi] = simp(imag(den(k,l)), var_name^deg);
                tmp_d = dnr + dni * %i;

                if ~isreal(tmp_n) & real(tmp_n) == 0 & ~isreal(tmp_d) & real(tmp_d) == 0 then
                    tmp_n = real(imag(tmp_n));
                    tmp_d = real(imag(tmp_d));
                end

                if tmp_n == tmp_d & tmp_n <> 1 & tmp_d <> 1 then
                    tmp_n = 1*var_name^0;
                    tmp_d = 1*var_name^0;
                end
            end

            n(k,l) = tmp_n;
            d(k,l) = tmp_d;

        end
    end
    h(2) = n;
    h(3) = d;
endfunction
