// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Create a new axis-by-axis simplex and compute the 
// size by various methods.
x0 = [0.0 0.0];
si = optimsimplex_new ( "axes" , x0 );
methodlist = [
"sigmaplus" 
"sigmaminus"
"Nash"
"diameter"
];
for i = 1:size(methodlist,"*")
  m = methodlist ( i );
  ss = optimsimplex_size ( si , m );
  mprintf ( "%s: %f\n", m , ss );
end
optimsimplex_destroy(si)

