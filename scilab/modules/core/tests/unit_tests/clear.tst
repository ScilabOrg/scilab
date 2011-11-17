// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

A = 2;
B = 4;
C = 6;
clear("B")
assert_checkequal(isdef("B"), %f);
assert_checkequal(isdef("A"), %t);
assert_checkequal(isdef("C"), %t);

clear();
assert_checkequal(isdef("A"), %f);
assert_checkequal(isdef("C"), %f);

function test_clear()
  A = 6;
  clear("A")
  assert_checkequal(isdef("A"), %f);
endfunction
test_clear();
A = 3;
assert_checkequal(isdef("A"), %t);
assert_checkequal(A, 3);

ierr = execstr("clear SCI", "errcatch");
msg_err = lasterror();
assert_checkequal(ierr, 13);
msg_ref = msprintf(gettext("Redefining permanent variable.\n"));
assert_checkequal (msg_err, msg_ref);

ierr = execstr("clear(1)", "errcatch");
assert_checkequal(ierr, 201);
msg_ref = msprintf(gettext("%s: Wrong type for argument %d: Valid variable name expected.\n"), "clear", 1);
assert_checkerror ("clear(1)", msg_ref);

ierr = execstr("clear toto", "errcatch");
assert_checkequal(ierr, 0);

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


ierr = execstr("clear(""to+to"")", "errcatch");
assert_checkequal(ierr, 201);
msg_ref = msprintf(gettext("%s: Wrong value for argument %d: Valid variable name expected.\n"), "clear", 1);
assert_checkerror ("clear(""to+to"")", msg_ref);
