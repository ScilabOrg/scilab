// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
// Copyright (C) 2009 - INRIA - Michael Baudin, Antoine Elias
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 2775 -->
//
// <-- JVM NOT MANDATORY -->
// <-- Bugzilla URL -->
// <-- NOT FIXED -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2775
//


//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if ( type(computed) == 1 & type(expected) == 1 ) then
    csize = size (computed,"*")
    esize = size (expected,"*")
    [cnonan , cnumb] = thrownan(computed)
    [enonan , enumb] = thrownan(expected)
    if ( and(enonan == cnonan) & and(enumb == cnumb) ) then
      flag = 1;
    else
      flag = 0;
    end
  else
    if computed==expected then
      flag = 1;
    else
      flag = 0;
    end
  end
  if flag <> 1 then pause,end
endfunction

// Bibliography :
// "IEEE Standard for Floating-Point Arithmetic"
// IEEE Std 754-2008
// 29 August 2008
// (Revision of IEEE Std 754-1985)

res_1 = 0.0 * %nan;
res_2 = %nan * 0.0;

assert_equal ( res_1 , %nan );
assert_equal ( res_2 , %nan );


