// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function Pop_init = init_ga_default(popsize,param)
    if ~isdef("param","local") then
        param = [];
    end

    // We deal with some parameters to take into account the boundary of the domain and the neighborhood size
    [Dim,err]       = get_param(param,"dimension",2);
    [MinBounds,err] = get_param(param,"minbound",-2*ones(1,Dim));
    [MaxBounds,err] = get_param(param,"maxbound",2*ones(1,Dim));

    // Pop_init must be a list()
    Pop_init = list();
    for i=1:popsize
        Pop_init(i) = (MaxBounds - MinBounds).*grand(size(MaxBounds,1),size(MaxBounds,2),"def") + MinBounds;
    end
endfunction
