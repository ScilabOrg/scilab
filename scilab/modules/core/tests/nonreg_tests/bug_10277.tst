// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 10277 -->
//
// <-- JVM NOT MANDATORY -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10277
//
// <-- Short Description -->
// clear() did not allow overload on mlist

// overload on mlist as input argument
M = mlist(['V','name','value'],['a','b';'c' 'd'],[1 2; 3 4]);
assert_checkequal(isdef('M'), %t);
      
function %V_clear(varargin)
  [lhs,rhs] = argn(0)
   if type(varargin(1)) == 17 then
     disp('call %V_clear overload: OK')
   else
     disp('call %V_clear overload: NOK')
   end
endfunction
      
clear(M)
assert_checkequal(isdef('M'), %f);