// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- JVM NOT MANDATORY -->

// Run with test_run('core, 'clear')

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

A = 2;
B = 4;
C = 6;
D = 8;
clear( ["A" "B" "C" "D"] );
assert_checkequal(isdef("A"), %f);
assert_checkequal(isdef("B"), %f);
assert_checkequal(isdef("C"), %f);
assert_checkequal(isdef("D"), %f);

A = 2;
B = 4;
C = 6;
D = 8;
clear( ["A" ; "B" ; "C" ; "D"] );
assert_checkequal(isdef("A"), %f);
assert_checkequal(isdef("B"), %f);
assert_checkequal(isdef("C"), %f);
assert_checkequal(isdef("D"), %f);

A = 2;
B = 4;
C = 6;
D = 8;
clear( ["A" "B" ; "C" "D"] );
assert_checkequal(isdef("A"), %f);
assert_checkequal(isdef("B"), %f);
assert_checkequal(isdef("C"), %f);
assert_checkequal(isdef("D"), %f);

A = 2;
B = 4;
C = 6;
D = 8;
clear( ["A" "B"], ["C";"D"] );
assert_checkequal(isdef("A"), %f);
assert_checkequal(isdef("B"), %f);
assert_checkequal(isdef("C"), %f);
assert_checkequal(isdef("D"), %f);


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
assert_checkequal(ierr, 207);
msg_ref = msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "clear", 1);
assert_checkerror ("clear(1)", msg_ref);

ierr = execstr("clear toto", "errcatch");
assert_checkequal(ierr, 0);
