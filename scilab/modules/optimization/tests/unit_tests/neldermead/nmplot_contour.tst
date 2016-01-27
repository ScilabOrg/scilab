// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
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

// <-- ENGLISH IMPOSED -->


function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

// Use 20 points in X and 20 points in Y
NP = 20;

//
// Test a basic contour plot
//
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",rosenbrock);
xmin = -2.0;
xmax = 2.0;
ymin = -2.0;
ymax = 2.0;
nx = NP;
ny = NP;
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin , xmax , ymin , ymax , nx , ny );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , [1 10 100 500 1000 2000] );
drawnow();
close(f);
nm = nmplot_destroy(nm);


// Test with a function for which the column orientation matters
function [ y , index ] = myquad ( x , index )
  y = x' * x;
endfunction

// Use 20 points in X and 20 points in Y
NP = 20;

//
// Test a basic contour plot
//
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",myquad);
xmin = -2.0;
xmax = 2.0;
ymin = -2.0;
ymax = 2.0;
nx = NP;
ny = NP;
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin , xmax , ymin , ymax , nx , ny );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , linspace(0,8,20) );
drawnow();
close(f);
nm = nmplot_destroy(nm);


