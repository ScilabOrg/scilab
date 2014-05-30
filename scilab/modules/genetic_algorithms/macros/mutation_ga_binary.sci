// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2014 - Pierre-Aime Agnel
// Copyright (C) 2014 - Michael Baudin <michael.baudin@contrib.scilab.org>
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [Mut_Indiv, pos] = mutation_ga_binary(Indiv, param)
    if ~isdef("param", "local") then
        param = [];
    end
    // We deal with some parameters to take into account the boundary of the domain and the neighborhood size
    [BinLen, err]     = get_param(param, "binary_length", 8);
    [MultiMut, err]   = get_param(param, "multi_mut", %F);
    if MultiMut
        [MultiMutNb, err] = get_param(param, "multi_mut_nb", 2);
    else
        MultiMutNb = 1;
    end

    dim = length(Indiv);
    pos = grand(1, MultiMutNb, "uin", 1, dim);
    pos = unique(pos);
    Mut_Indiv = Indiv;
    Mut_Indiv = strsplit(Indiv);
    Mut_Indiv(pos) = string(-1 * (eval(Mut_Indiv(pos)) - 1)); // Inverts 0s and 1s on mutation locations
    Mut_Indiv = strcat(Mut_Indiv);
endfunction
