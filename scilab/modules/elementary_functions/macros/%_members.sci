// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %_members()
    // Demo file for call to members() with no input argument

    messagebox(_("Demo for function members"), "Demo", "demo", ["Continue" "Cancel"], "modal");

    disp("-------------------------");

    disp("Null vectors");
    [nb, loc] = members([], [], %t);
    disp(nb,"nb",loc,"loc", "[nb, loc] = members([], [])");
    disp("------------");

    disp("Random vectors");
    A = [1 8 4 5 2 1];
    B = [9 7 4 2 1 4];

    [nb, loc] = members(A, B, %t);
    disp("B = [9 7 4 2 1 4]", "A = [1 8 4 5 2 1]");
    disp(nb,"nb",loc,"loc", "[nb, loc] = members(A, B, %t)");
    disp("-------------");

    disp("Matrices");
    A = [ 5 0 1 4 1 ;
    0 5 3 1 9 ;
    2 0 1 6 1 ;
    0 2 2 2 2 ;
    2 0 8 1 7 ;
    6 7 1 9 3 ];
    B = [4 8 1 ;
    1 0 2 ;
    6 2 3 ;
    2 9 4 ;
    1 2 5 ;
    3 0 6 ];

    [nb, loc] = members(A, B, %t);
    disp("B = [4 8 1; 1 0 2; 6 2 3; 2 9 4; 1 2 5; 3 0 6]", "A = [5 0 1 4 1; 0 5 3 1 9; 2 0 1 6 1; 0 2 2 2 2; 2 0 8 1 7; 6 7 1 9 3]");
    disp(nb,"nb",loc,"loc", "[nb, loc] = members(A, B, %t)");
    disp("-------------");

    disp("Strings");

    A = ["elt1" "elt3" "elt4"];
    B = ["elt5" "elt6" "elt2" "elt1" "elt3"];

    [nb, loc] = members(A, B, %t);
    disp("B = [""elt5"" ""elt6"" ""elt2"" ""elt1"" ""elt3""]", "A = [""elt1"" ""elt3"" ""elt4""]");
    disp(nb,"nb",loc,"loc", "[nb, loc] = members(A, B, %t)");
    disp("-------------");

endfunction
