// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [Crossed_Indiv1, Crossed_Indiv2] = crossover_ga_default(Indiv1,Indiv2,param)
    if ~isdef("param","local") then
        param = [];
    end

    // We deal with some parameters to take into account the boundary of the domain and the neighborhood size
    [Beta,err]      = get_param(param,"beta",0);
    [MinBounds,err] = get_param(param,"minbound",-2*ones(size(Indiv1,1),size(Indiv1,2)));
    [MaxBounds,err] = get_param(param,"maxbound",2*ones(size(Indiv1,1),size(Indiv1,2)));

    mix = (1 + 2*Beta)*grand(1,1,"def") - Beta;
    Crossed_Indiv1 =     mix*Indiv1 + (1-mix)*Indiv2;
    Crossed_Indiv2 = (1-mix)*Indiv1 +     mix*Indiv2;

    Crossed_Indiv1 = max(min(Crossed_Indiv1, MaxBounds),MinBounds);
    Crossed_Indiv2 = max(min(Crossed_Indiv2, MaxBounds),MinBounds);
endfunction
