// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9627 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9627
//
// <-- Short Description -->
// The optimsimplex_set*() functions did not check any arguments:
//

// optimsimplex_check
s1 = optimsimplex_new ();
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_check", 1);
assert_checkerror("s1 = optimsimplex_check(""1"");",refMsg);
assert_checkerror("s1 = optimsimplex_check(%t);",refMsg);

// optimsimplex_getall
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getall", 1);
assert_checkerror("s1 = optimsimplex_getall(""1"");",refMsg);
assert_checkerror("s1 = optimsimplex_getall(%t);",refMsg);

// optimsimplex_getallfv
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getallfv", 1);
assert_checkerror("s1 = optimsimplex_getallfv(""1"");",refMsg);
assert_checkerror("s1 = optimsimplex_getallfv(%t);",refMsg);

// optimsimplex_getallx
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getallx", 1);
assert_checkerror("s1 = optimsimplex_getallx(""1"");",refMsg);
assert_checkerror("s1 = optimsimplex_getallx(%t);",refMsg);

// optimsimplex_getfv
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getfv", 1);
assert_checkerror("s1 = optimsimplex_getfv(""1"");",refMsg);
assert_checkerror("s1 = optimsimplex_getfv(%t);",refMsg);

// optimsimplex_getn
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getn", 1);
assert_checkerror("s1 = optimsimplex_getn(""1"");",refMsg);
assert_checkerror("s1 = optimsimplex_getn(%t);",refMsg);

// optimsimplex_getnbve
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getnbve", 1);
assert_checkerror("s1 = optimsimplex_getnbve(""1"");",refMsg);
assert_checkerror("s1 = optimsimplex_getnbve(%t);",refMsg);

// optimsimplex_getve
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getve", 1);
assert_checkerror("s1 = optimsimplex_getve(""1"");",refMsg);
assert_checkerror("s1 = optimsimplex_getve(%t);",refMsg);

// optimsimplex_getx
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getx", 1);
assert_checkerror("s1 = optimsimplex_getx(""1"");",refMsg);
assert_checkerror("s1 = optimsimplex_getx(%t);",refMsg);

// optimsimplex_setall
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setall", 1);
refMsg2 = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "optimsimplex_setall", 2);
assert_checkerror("s1 = optimsimplex_setall(1, s1);",refMsg);
assert_checkerror("s1 = optimsimplex_setall(""s1"", s1);",refMsg);
assert_checkerror("s1 = optimsimplex_setall(s1, %i);",refMsg2);
assert_checkerror("s1 = optimsimplex_setall(s1, ""s1"");",refMsg2);

// optimsimplex_setallfv
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setallfv", 1);
refMsg2 = msprintf(_("%s: Wrong type for input argument #%d: A real vector expected.\n"), "optimsimplex_setallfv", 2);
assert_checkerror("s1 = optimsimplex_setallfv(1, [1 2]);",refMsg);
assert_checkerror("s1 = optimsimplex_setallfv(""s1"", [1 2]);",refMsg);
assert_checkerror("s1 = optimsimplex_setallfv(s1, ""1"");",refMsg2);
assert_checkerror("s1 = optimsimplex_setallfv(s1, %t);",refMsg2);

// optimsimplex_setallx
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setallx", 1);
refMsg2 = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "optimsimplex_setallx", 2);
assert_checkerror("s1 = optimsimplex_setallx(1, [1 2]);",refMsg);
assert_checkerror("s1 = optimsimplex_setallx(""s1"", [1 2]);",refMsg);
assert_checkerror("s1 = optimsimplex_setallx(s1, ""1"");",refMsg2);
assert_checkerror("s1 = optimsimplex_setallx(s1, %t);",refMsg2);

// optimsimplex_setfv
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setfv", 1);
refMsg2 = msprintf(_("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "optimsimplex_setfv", 2);
refMsg3 = msprintf(_("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optimsimplex_setfv", 2);
refMsg4 = msprintf(_("%s: Wrong value for input argument #%d: An integer expected.\n"), "optimsimplex_setfv", 2);
refMsg5 = msprintf(_("%s: Wrong type for input argument #%d: A real vector expected.\n"), "optimsimplex_setfv", 3);
assert_checkerror("s1 = optimsimplex_setfv(1, 1, 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setfv(""s1"", 1, 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setfv(s1, ""1"", 1);",refMsg2);
assert_checkerror("s1 = optimsimplex_setfv(s1, [1 1], 1);",refMsg3);
assert_checkerror("s1 = optimsimplex_setfv(s1, 1.5, 1);",refMsg4);
assert_checkerror("s1 = optimsimplex_setfv(s1, 1, ""1"");",refMsg5);
assert_checkerror("s1 = optimsimplex_setfv(s1, 1, %t);",refMsg5);

// optimsimplex_setn
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setn", 1);
refMsg2 = msprintf(_("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "optimsimplex_setn", 2);
refMsg3 = msprintf(_("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optimsimplex_setn", 2);
refMsg4 = msprintf(_("%s: Wrong value for input argument #%d: An integer expected.\n"), "optimsimplex_setn", 2);
assert_checkerror("s1 = optimsimplex_setn(1, 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setn(""s1"", 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setn(s1, ""1"");",refMsg2);
assert_checkerror("s1 = optimsimplex_setn(s1, [1 1]);",refMsg3);
assert_checkerror("s1 = optimsimplex_setn(s1, 1.5);",refMsg4);

// optimsimplex_setnbve
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setnbve", 1);
refMsg2 = msprintf(_("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "optimsimplex_setnbve", 2);
refMsg3 = msprintf(_("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optimsimplex_setnbve", 2);
refMsg4 = msprintf(_("%s: Wrong value for input argument #%d: An integer expected.\n"), "optimsimplex_setnbve", 2);
assert_checkerror("s1 = optimsimplex_setnbve(1, 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setnbve(""s1"", 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setnbve(s1, ""1"");",refMsg2);
assert_checkerror("s1 = optimsimplex_setnbve(s1, [1 1]);",refMsg3);
assert_checkerror("s1 = optimsimplex_setnbve(s1, 1.5);",refMsg4);

// optimsimplex_setve
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setve", 1);
refMsg2 = msprintf(_("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "optimsimplex_setve", 2);
refMsg3 = msprintf(_("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optimsimplex_setve", 2);
refMsg4 = msprintf(_("%s: Wrong value for input argument #%d: An integer expected.\n"), "optimsimplex_setve", 2);
refMsg5 = msprintf(_("%s: Wrong type for input argument #%d: A real vector expected.\n"), "optimsimplex_setve", 3);
refMsg6 = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "optimsimplex_setve", 4);
assert_checkerror("s1 = optimsimplex_setve(1, 1, 1, 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setve(""s1"", 1, 1, 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setve(s1, ""1"", 1, 1);",refMsg2);
assert_checkerror("s1 = optimsimplex_setve(s1, [1 1], 1, 1);",refMsg3);
assert_checkerror("s1 = optimsimplex_setve(s1, 1.5, 1, 1);",refMsg4);
assert_checkerror("s1 = optimsimplex_setve(s1, 1, ""1"", 1);",refMsg5);
assert_checkerror("s1 = optimsimplex_setve(s1, 1, %t, 1);",refMsg5);
assert_checkerror("s1 = optimsimplex_setve(s1, 1, 1, ""1"");",refMsg6);
assert_checkerror("s1 = optimsimplex_setve(s1, 1, 1, %t);",refMsg6);

// optimsimplex_setx
refMsg = msprintf(_("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setx", 1);
refMsg2 = msprintf(_("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "optimsimplex_setx", 2);
refMsg3 = msprintf(_("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optimsimplex_setx", 2);
refMsg4 = msprintf(_("%s: Wrong value for input argument #%d: An integer expected.\n"), "optimsimplex_setx", 2);
refMsg5 = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "optimsimplex_setx", 3);
assert_checkerror("s1 = optimsimplex_setx(1, 1, 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setx(""s1"", 1, 1);",refMsg);
assert_checkerror("s1 = optimsimplex_setx(s1, ""1"", 1);",refMsg2);
assert_checkerror("s1 = optimsimplex_setx(s1, [1 1], 1);",refMsg3);
assert_checkerror("s1 = optimsimplex_setx(s1, 1.5, 1);",refMsg4);
assert_checkerror("s1 = optimsimplex_setx(s1, 1, ""1"");",refMsg5);
assert_checkerror("s1 = optimsimplex_setx(s1, 1, %t);",refMsg5);
