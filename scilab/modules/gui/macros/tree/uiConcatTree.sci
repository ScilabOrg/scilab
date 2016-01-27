// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function myNewTree = uiConcatTree(tree1, tree2)

    [lhs,rhs]=argn(0);


    //Input arguments checking
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "uiConcatTree",2));
        return;
    end

    // Check 1st and 2nd inputs : tree1 & tree2
    if rhs == 2 then
        if (typeof(tree1) == "uitree") then
            myTree1 = tree1;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiConcatTree",1));
            return;
        end

        if (typeof(tree2) == "uitree") then
            myTree2 = tree2;
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiConcatTree",2));
            return;
        end
    end

    tree1($+1) = tree2;
    myNewTree = tree1;

endfunction
