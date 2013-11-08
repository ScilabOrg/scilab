// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function demo_nmplot_qvariable2()

    filename = "nmplot_quadratic.variable2.sce";
    dname = get_absolute_file_path(filename);

    mprintf(_("Illustrates that the Nelder-Mead algorithm performs correctly on a badly quadratic test case.\n"));
    mprintf(_("Defining quadratic function ...\n"));
    a = 100.0;
    function [ y , index ] = quadratic ( x , index )
        y = a * x(1)^2 + x(2)^2;
    endfunction
    function y = quadraticC ( x1 , x2 )
        [ y , index ] = quadratic ( [x1 , x2] , 2 )
    endfunction

    mprintf(_("Creating nmplot object...\n"));
    nm = nmplot_new ();
    nm = nmplot_configure(nm, "-numberofvariables",2);
    nm = nmplot_configure(nm, "-function",quadratic);
    nm = nmplot_configure(nm, "-x0",[10.0 10.0]');
    nm = nmplot_configure(nm, "-maxiter",400);
    nm = nmplot_configure(nm, "-maxfunevals",400);
    nm = nmplot_configure(nm, "-tolxmethod",%f);
    nm = nmplot_configure(nm, "-tolsimplexizerelative",1.e-8);
    nm = nmplot_configure(nm, "-simplex0method","spendley");

    //
    // Setup output files
    //
    simplexfn = TMPDIR + filesep() + "history.simplex.txt";
    nm = nmplot_configure(nm, "-simplexfn",simplexfn);
    //
    // Perform optimization
    //
    mprintf(_("Searching (please wait) ...\n"));
    nm = nmplot_search(nm);
    //
    // Print a summary
    //
    exec(fullfile(dname,"nmplot_summary.sci"),-1);
    nmplot_summary(nm)
    //
    // Plot the contours of the cost function and the simplex history
    mprintf(_("Plotting contour (please wait) ...\n"));
    nm = nmplot_configure(nm, "-verbose", 0);
    xmin = -5.0 ;
    xmax = 12.0 ;
    ymin = -2.0 ;
    ymax = 12.0 ;
    nx = 50 ;
    ny = 50;
    scf()
    drawlater();
    xdata=linspace(xmin,xmax,nx);
    ydata=linspace(ymin,ymax,ny);
    contour ( xdata , ydata , quadraticC , [10.0 50 100 1000 2000 5000 10000 20000] )
    nmplot_simplexhistory ( nm );
    drawnow();
    demo_viewCode(filename);
    //
    // Clean-up
    deletefile(simplexfn);
    nm = nmplot_destroy(nm);
    mprintf(_("End of demo.\n"));
endfunction

demo_nmplot_qvariable2();
clear demo_nmplot_qvariable2;




