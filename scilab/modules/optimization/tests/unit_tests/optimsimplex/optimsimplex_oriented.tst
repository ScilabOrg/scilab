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


function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

// With base simplex and no function
simplex = [...
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
s1 = optimsimplex_new();
s1 = optimsimplex_setall ( s1 , simplex );
s2 = optimsimplex_new ( "oriented" , s1 );
computed = optimsimplex_getall ( s2 );
expected = [...
24.2 -1.2 1.0
0.0 -1.7 1.0
0.0 -1.2 0.5
];
assert_checkalmostequal ( computed , expected , %eps );
s1 = optimsimplex_destroy(s1);
s2 = optimsimplex_destroy(s2);

// With basic function
simplex = [...
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
s1 = optimsimplex_new();
s1 = optimsimplex_setall ( s1 , simplex );
s2 = optimsimplex_new ( "oriented" , s1 , rosenbrock );
computed = optimsimplex_getall ( s2 );
expected = [...
24.2 -1.2 1.0
364.5 -1.7 1.0
93.2 -1.2 0.5
];
assert_checkalmostequal ( computed , expected , %eps );
s1 = optimsimplex_destroy(s1);
s2 = optimsimplex_destroy(s2);
//
// Test with an additional argument
//
myobj = tlist(["T_MYSTUFF","nb"]);
myobj.nb = 0;
function [ y , myobj ] = mycostf ( x , myobj )
  y = rosenbrock(x);
  myobj.nb = myobj.nb + 1
endfunction
simplex = [...
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
s1 = optimsimplex_new();
s1 = optimsimplex_setall ( s1 , simplex );
[s2,myobj] = optimsimplex_new ( "oriented" , s1 , mycostf , myobj );
computed = optimsimplex_getall ( s2 );
expected = [...
24.2 -1.2 1.0
364.5 -1.7 1.0
93.2 -1.2 0.5
];
assert_checkalmostequal ( computed , expected , %eps );
assert_checkequal ( myobj.nb , 3 );
s1 = optimsimplex_destroy(s1);
s2 = optimsimplex_destroy(s2);

