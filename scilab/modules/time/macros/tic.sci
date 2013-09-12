// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - V.C & A.C
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function tic()
    // tic() start a stopwatch timer.
    // The sequence of commands tic(), operation, toc()
    // prints the number of milliseconds required for the operation.


    global TICTOC;

    TICTOC=getdate();

endfunction
