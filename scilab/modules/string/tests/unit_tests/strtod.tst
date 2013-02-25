// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// strtod
//===============================
N = 666.666;
STREND = " is a double";
STR = string(N)+STREND;
d = strtod(STR);
assert_checkequal(d, N);
[d,s] = strtod(STR);
assert_checkequal(d, N);
assert_checkequal(s, STREND);
//===============================
STR = "1";
[d,s] = strtod(STR);
assert_checkequal(s, "");
assert_checkequal(d, 1);
//===============================
STR = "string";
[d,s] = strtod(STR);
assert_checkequal(s, STR);
assert_checkequal(d, %nan);
//===============================
STR1 = "1A";
STR2 = "2B";
STR3 = "3C";
STR4 = "4d";
STR = [STR1,STR2;STR3,STR4];
[d,s] = strtod(STR);
assert_checkequal(s, ["A","B";"C","d"]);
assert_checkequal(d, [1,2;3,4]);
//===============================
[d,s] = strtod("");
assert_checkequal(s, "");
assert_checkequal(d, %nan);
//===============================
[d,s] = strtod("  ");
assert_checkequal(s, "  ");
assert_checkequal(d, %nan);
//===============================
