//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2481 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2481
//
// <-- Short Description -->
//   I get warned of a stack problem when I enter the input variables given 
//  in the 'steps to reproduce the bug'. It prevents me from using the rest 
//  of my code, as I then get the error message described below. Any clues? 
//  Apparently "sigma.loc = zeros(3,4)" is crucial, entering a random 
//  matrix does not have the same effect. It makes me believe it is a bug 
//  in the try-catch primitive.

function x=foo
  x=%t	
  try
    r = 1 / zeros(1)
    x=%f
  end
endfunction

function x=rf()
  strength = ones(1,2);
  sigma = zeros(1,2);
  x=%t
  try
    z=strength ./ sigma(1,1);
    x=%f
  end
endfunction

if ~foo() then pause,end
if ~rf() then pause,end
