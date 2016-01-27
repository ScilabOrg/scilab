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

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->




function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.0 2.0]');
nm = neldermead_configure(nm,"-simplex0method","spendley");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_search(nm, "off");
simplex = neldermead_get(nm,"-simplex0");
computed = optimsimplex_getallx( simplex );
expected = [
1.    2.
1.96592582628907    2.25881904510252
1.25881904510252  2.96592582628907
];
assert_checkalmostequal ( computed, expected, 10 * %eps );
nm = neldermead_destroy(nm);
clear nm;

