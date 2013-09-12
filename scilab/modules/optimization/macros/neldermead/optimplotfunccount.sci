// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// optimplotfunccount --
//   Emulate the optimplotfunccount command of Matlab.
//   Plot the function value during an optimization process.
// Arguments, input
//   x : the current point
//   optimValues : a tlist which contains the following fields
//     funcCount : the number of function evaluations
//     fval : the current function value
//     iteration : the current iteration
//     procedure : a string containing the current type of step
//  state : the current state of the algorithm
//    "init", "iter", "done"
// Notes:
//   The algorithm is the following.
//   At initialization of the algorithm, create an empty
//   graphic plot, retrieve the handle, and store a special
//   key "optimplotfunccount" in the user_data field of the handle.
//   When the plot is to update, this key is searched so that the
//   correct plot can be update (and not another).
//
function optimplotfunccount ( x , optimValues , state )
    if ( state == "init" ) then
        opfvh = scf();
        plot ( 0 , optimValues.funccount );
        opfvh.user_data = "optimplotfunccount";
        opfvh.children.x_label.text = "Iteration";
        opfvh.children.y_label.text = "Function evaluations";
        opfvh.children.title.text = msprintf ( "Total Function Evaluations: %d", optimValues.funccount )
        opfvh.children.children.children.mark_mode = "on";
        opfvh.children.children.children.mark_style = 5;
        opfvh.children.children.children.mark_size = 10;
        opfvh.children.children.children.mark_background = 6;
    else
        opfvh = findobj ( "user_data" , "optimplotfunccount" );
        gg = opfvh.children.children;
        // Update data
        gg.children.data($+1,1:2) = [optimValues.iteration optimValues.funccount];
        // Compute new bounds
        itermin = 0;
        itermax = optimValues.iteration;
        fmin = min(gg.children.data(:,2));
        fmax = max(gg.children.data(:,2));
        // Update bounds
        opfvh.children.data_bounds = [
        itermin fmin
        itermax fmax
        ];
        // Update title
        opfvh.children.title.text = msprintf ( "Total Function Evaluations: %d", optimValues.funccount )
    end
endfunction

