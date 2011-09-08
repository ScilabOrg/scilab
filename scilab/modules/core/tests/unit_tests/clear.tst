// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

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

A = 3;
function test_clear()
  A = 6;
  clear("A")
  assert_checkequal(isdef("A"), %f);
endfunction
assert_checkequal(isdef("A"), %t);
assert_checkequal(A, 3);

ierr = execstr("clear SCI", "errcatch");
assert_checkequal(ierr, 13);

ierr = execstr("clear(1)", "errcatch");
assert_checkequal(ierr, 201);

ierr = execstr("clear toto", "errcatch");
assert_checkequal(ierr, 0);
