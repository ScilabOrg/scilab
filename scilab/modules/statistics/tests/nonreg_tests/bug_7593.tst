// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7593 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7593
//
// <-- Short Description -->
// The function st_deviation was merged into stdev (they were duplicates),
// and msd is now accessible through the third input argument of stdev

x = matrix([5 2:8], 2, 4);

assert_checkalmostequal(stdev(x,"*",%t), 1.870828693387);
assert_checkalmostequal(norm(stdev(x,1,%t)-[3 1 1 1]/2),   0);
assert_checkalmostequal(norm(stdev(x,"r",%t)-[3 1 1 1]/2), 0);
assert_checkalmostequal(norm(stdev(x,2,%t)-sqrt([2;5])),   0);
assert_checkalmostequal(norm(stdev(x,"c",%t)-sqrt([2;5])), 0);

refMsg = msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"stdev",3,"%f","%t");
assert_checkerror("stdev(x, ""*"", [1 1])",   refMsg);
assert_checkerror("stdev(x, ""*"", ""msd"")", refMsg);
