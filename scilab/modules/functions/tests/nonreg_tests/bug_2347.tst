// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2347 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2347
//
// <-- Short Description -->
// fun2string does not correctly indent several 'case' clauses.
//    
function testfun(a)
select a
case 1 then
disp('a is 1')
case 2 then
disp('a is')
disp('2')
case 3 then
disp('3')
else
disp('a is greater')
end
endfunction
s=fun2string(testfun,'testf');                    
ref=["function []=testf(a)";
     "  select a";
     "  case 1 then";
     "    disp(''a is 1'')";
     "  case 2 then";
     "    disp(''a is'')";
     "    disp(''2'')";
     "  case 3 then";
     "    disp(''3'')";
     "  else";
     "    disp(''a is greater'')";
     "  end,";
     "endfunction"]
if or(s<>ref) then pause,end
