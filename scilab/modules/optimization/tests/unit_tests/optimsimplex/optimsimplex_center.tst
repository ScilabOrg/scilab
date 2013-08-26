// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->

//
// Check behaviour with default settings.
//

function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

coords = [...
    0.    0.  
    3.    0.  
    0.    6.  
];
s1 = optimsimplex_new ( coords );
sicenter = optimsimplex_center ( s1 );
assert_checkalmostequal ( sicenter , [1.0 2.0], %eps );
s1 = optimsimplex_destroy(s1);


