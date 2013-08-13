// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_plot2d_1()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("plot2d1_1.dem.sce");

    // DEMO START

    my_plot_desc          = "plot2d1 log scale";
    my_handle.figure_name = my_plot_desc;

    plot2d1("enl",1,(1:10:10000)');
    xtitle(my_plot_desc,"t","y  log scale");
    xgrid(3);

    // DEMO END
endfunction

demo_plot2d_1();
clear demo_plot2d_1;