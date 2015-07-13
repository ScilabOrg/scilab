// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent Couvert
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function C=tree2code(P,bprettyprintformat)
    // Convert a tree returned by macr2tree() to a macro code
    // Input:
    // - P: 'program' tlist
    // - bprettyprintformat: boolean value, if FALSE (default value), generated code is not formated else it is
    // Output:
    // - C: character string matrix (colum vector)

    // Verify good type for input
    if typeof(P)<>"program" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: ''%s'' expected.\n"),"tree2code",2,"program tree"));
    end

    // Default value
    rhs = argn(2);
    if rhs==1 then
        bprettyprintformat=%F
    end

    C=""

    // Function prototype
    lhsstr=[]
    rhsstr=[]
    if P.name<>"" then // Not a batch file
        for k=1:size(P.outputs)
            lhsstr=[lhsstr,expression2code(P.outputs(k))]
        end
        lhsstr="["+strcat(lhsstr,",")+"]"

        for k=1:size(P.inputs)
            rhsstr=[rhsstr,expression2code(P.inputs(k))]
        end
        rhsstr="("+strcat(rhsstr,",")+")"

        C="function "+lhsstr+" = "+P.name+rhsstr;
    end

    I=P.statements

    // To avoid to have some instructions on the first line (function prototype)
    if I(1)<>list("EOL") then
        if bprettyprintformat then
            C=cat_code(C,instruction2code(list("EOL"),%T));
        else
            C=cat_code(C,", ");
        end
    end

    // For each statement, generate corresponding code
    for i=1:size(I)-1
        if bprettyprintformat then
            C=cat_code(C,"  "+instruction2code(I(i),bprettyprintformat));
        else
            C=cat_code(C,instruction2code(I(i),bprettyprintformat));
        end
        C = format_txt(C,I(i),bprettyprintformat,I(i+1));
    end

    if bprettyprintformat then
        C=cat_code(C,"  "+instruction2code(I($),bprettyprintformat));
    else
        C=cat_code(C,instruction2code(I($),bprettyprintformat));
    end

    // To avoid to have some instructions on the last line (endfunction)
    if bprettyprintformat && I($)<>list("EOL") then
        C=cat_code(C,instruction2code(list("EOL"),%T));
    end

    if P.name<>"" then // Not a batch file
        C=cat_code(C,"endfunction");
    end
endfunction

