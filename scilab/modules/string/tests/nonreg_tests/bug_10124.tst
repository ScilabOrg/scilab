// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10124 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10124
//
// <-- Short Description -->
// strsplit crashed with wrong input arguments
//


A = ["BCUCCGACUGCAUCU";
"CUCCGACUGCACCGACUGCAUCU";
"CUCCGACUGCACCGACUGCAUCU";
"CUCCGACUG";
"CCGACUGCACCGACUGCAUCU";
"CUCCGACUGCACCGACUGCAUCU";
"CUCCUCCGACUGCAUCU";
"CUCCGACUGCACCGACUGCAUCU";
"CUCCGACUGCACCGACUGCAUCU";
"CUCCGACUG";
"CCGACUGCACCGACUGCAUCU";
"CUCCGA"];

B = A.';

msgerr = msprintf(gettext("%s: Wrong size for input argument #%d: A single string expected.\n"),"strsplit", 1);
assert_checkerror("C = strsplit(A,3:3:sum(length(B))-1)", msgerr);

for i=1:50
  [a, b] = strsplit([]);
  assert_checkequal(a, []);
  assert_checkequal(b, '');
end

for i=1:50
  [a, b] = strsplit([], '');
  assert_checkequal(a, []);
  assert_checkequal(b, '');
end

for i=1:50
  [a, b] = strsplit('', '');
  assert_checkequal(a, '');
  assert_checkequal(b, '');
end
