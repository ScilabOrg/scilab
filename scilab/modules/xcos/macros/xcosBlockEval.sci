// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS <antoine.elias@scilab.org>
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

// Evaluate a block with the context input.
//
// @param interfaceAlias block interface function ( ex IN_f )
// @param blk the block
// @param context the context string
function blk = xcosBlockEval(interfaceAlias, blk, context)

    //replace scicos_getvalue by setvalue (call by interfaceAlias)
    %mprt = funcprot()
    funcprot(0)
    scicos_getvalue = setvalue;
    getvalue = setvalue;
    function message(txt)
        messagebox(..
        ["In block " + o.gui + ": " ; txt ; "current parameter value kept"],..
        "error","modal");
        [str,n,line,func]=lasterror();
        printf("do_eval: error %d - %s in %s at line %d\n", n, str, func, line);
    endfunction
    funcprot(%mprt)

    // define scicos variables
    %scicos_prob = %f
    %scicos_debug_gr = %f
    needcompile = 4;
    [modelica_libs, scicos_pal_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables();

    // allocate the context
    %scicos_context = struct();
    [%scicos_context, ierr] = script2var(context, %scicos_context)

    // Every parameter settings is done, perform block update

    //create a structure with the new context
    [new_blk, y, typ] = interfaceAlias("set", blk, []);
    blk = new_blk;
endfunction

