// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [] = bode_asymp(ss)
    // This function plots asymptotes for the magnitude and phase graphs.
    // Ref: http://forge.scilab.org/index.php/p/cpge/source/tree/HEAD/macros/REP_FREQ_pre_simulate.sci
    // "ss" is a linear system given by its transfer function representation and defined by syslin.

    if and(typeof(ss) <> ["state-space" "rational"]) then
        msg = _("Wrong type for argument #%d: Rational or State-space matrix expected.\n");
        error(msprintf(msg, 1))
        return;
    end

    siz = size(ss);

    for elem=1:siz(1) // Individually draw each asymptote of "ss" elements (row vector).

        h = ss(elem, 1);
        if typeof(h) == "state-space" then
            h = ss2tf(ss(elem, 1)); // "ss" is a linear system given by its state space representation and defined by syslin.
        end

        root_num = roots(h.num);
        root_den = roots(h.den);

        if (find(root_num==0))
            disp("Problem class of system is negative")
        end
        rac_nul = find(root_den==0);
        alpha = length(rac_nul);
        var = "s";
        domaine = h.dt;
        if type(domaine) == 1 then
            var = "z";
        elseif domaine == "c" then
            var = "s";
        elseif domaine == "d" then
            var = "z";
        end
        if domaine == [] then
            var = "s";
            if type(h.D) == 2 then
                var = varn(h.D);
            end
        end
        s = poly(0, var);
        try K = horner(h*s^alpha, 0); catch error(msprintf(_("%s: Problem evaluating the first argument.\n"), "bode")); end

        root_den(rac_nul) = []; // Removing the zeros

        if length([find(abs(root_num)>1e8) find(abs(root_num)<1e-8)]) > 0 then
            disp("Extreme root removed: "+string(root_num([find(abs(root_num)>1e8) find(abs(root_num)<1e-8)])'))
            root_num([find(abs(root_num)>1e8) find(abs(root_num)<1e-8)]) = [];
        end
        if length([find(abs(root_den)>1e8) find(abs(root_den)<1e-8)]) > 0 then
            disp("Extreme root removed: "+string(root_den([find(abs(root_den)>1e8) find(abs(root_den)<1e-8)])'))
            root_den([find(abs(root_den)>1e8) find(abs(root_den)<1e-8)]) = [];
        end

        i = 1;
        puls = [];
        order = [];
        while i<=length(root_num) // Real root
            if isreal(root_num(i), 0) then
                puls = [puls -root_num(i)];
                order = [order 1];
                i = i+1;
            else // Complex root
                xi = 1/sqrt(1+(imag(root_num(i))/real(root_num(i)))^2);
                puls = [puls -real(root_num(i))/xi];
                i = i+2;
                order = [order 2];
            end
        end
        i = 1;
        while i<=length(root_den) // Real root
            if isreal(root_den(i), 0) then
                puls = [puls -root_den(i)];
                order = [order -1];
                i = i+1;
            else // Complex root
                xi = 1/sqrt(1+(imag(root_den(i))/real(root_den(i)))^2);
                puls = [puls -real(root_den(i))/xi];
                order = [order -2];
                i = i+2;
            end
        end

        [puls, ind] = gsort(puls, "g", "i");
        order = order(ind);

        asym = [-20*alpha];
        phas = [-90*alpha];
        i = 1;
        while i<=length(puls)
            new_dir = asym($)+order(i)*20;
            asym = [asym new_dir];
            new_arg = phas($)+order(i)*90;
            phas = [phas new_arg];
            i = i+1;
        end

        // bode(h, w_min, w_max)
        fig = gcf();
        try sca(fig.children(2)); catch
            msg = _("ERROR: %s: bode() must be called before bode_asymp() and graphic window must still be running.\n");
            error(msprintf(msg, "bode"))
        end
        wmin = fig.children(1).data_bounds(1, 1); // Minimal frequence, w_min
        wmax = fig.children(1).data_bounds(2, 1); // Maximal frequence, w_max
        puls_to_plot = [];

        for p=real(puls)
            if p >= wmin & p <= wmax then
                puls_to_plot($+1) = p;
            end
        end
        puls = [wmin puls_to_plot' wmax];
        // End change DV

        eq_asymp = [20*log10(K/wmin^alpha)];
        puls_p = [];
        phas($+1) = phas($);
        eq_phas = [phas(1)];
        i = 2;
        while i<=length(puls)
            eq_asymp = [eq_asymp eq_asymp($)+asym(i-1)*log10(puls(i)/puls(i-1))];
            puls_p = [puls_p puls(i-1) puls(i)];
            eq_phas = [eq_phas phas(i-1) phas(i)];
            i = i+1;
        end

        // Adapt abscissae to current unit (Hertz or rad/s)
        if fig.children(1).x_label.text == "Frequency (Hz)" then
            puls   = puls./(2*%pi);
            puls_p = puls_p./(2*%pi);
        end
        plot(puls, eq_asymp, "b--");
        sca(fig.children(1));
        plot(puls_p, eq_phas(1:$-1), "r--");
    end
endfunction
