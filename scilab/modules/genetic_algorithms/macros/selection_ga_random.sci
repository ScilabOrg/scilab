// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [Pop_out,FObj_Pop_out,Efficiency,MO_Total_FObj_out] = selection_ga_random(Pop_in,Indiv1,Indiv2,FObj_Pop_in,FObj_Indiv1,FObj_Indiv2, ...
    MO_Total_FObj_in,MO_FObj_Indiv1,MO_FObj_Indiv2,param)

    [nargout,nargin] = argn();

    mo_is_defined = ~isempty("MO_Total_FObj_in");

    if ~isdef("param","local") then
        param = [];
    end

    [pressure,err] = get_param(param,"pressure",0.05);

    pop_size = length(Pop_in);

    Total_Pop  = lstcat(Pop_in, Indiv1, Indiv2);
    Total_FObj = [FObj_Pop_in' FObj_Indiv1' FObj_Indiv2']';

    // Normalization of the efficiency
    FObj_Pop_Max = max(Total_FObj);
    FObj_Pop_Min = min(Total_FObj);

    Efficiency = (1 - pressure) * (FObj_Pop_Max - Total_FObj)/max([FObj_Pop_Max - FObj_Pop_Min, %eps]) + pressure;

    // We select at random pop_size individuals
    Index_selected = ceil((length(Total_FObj)-1)*grand(pop_size,1,"def") + 1);

    // Extraction and selection of the phenotype
    FObj_Pop_out = Total_FObj(Index_selected);
    // Extraction and selection of the genotype
    Pop_out = list(Total_Pop(Index_selected));
    // Extraction of the efficiencies
    Efficiency = Efficiency(Index_selected);
    // Extraction of the multiobjective values (if defined)
    if mo_is_defined then
        // MO_Total_FObj for multiobjective function values
        MO_Total_FObj_out = [MO_Total_FObj_in' MO_FObj_Indiv1' MO_FObj_Indiv2']';
        MO_Total_FObj_out = MO_Total_FObj_out(Index_selected,:);
    end

    Total_Pop = list(); // Reinitialisation of Total_Pop
endfunction

