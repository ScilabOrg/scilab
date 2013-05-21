// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// unit tests for isrow function
// =============================================================================

// Constants
assert_checkequal(isrow([1 2 3]), %t);
assert_checkequal(isrow([1 2 3]'), %f);
assert_checkequal(isrow([1 2 3; 1 2 3]), %f);
assert_checkequal(isrow(1), %t);
assert_checkequal(isrow(rand(1,1,3)), %t);
assert_checkequal(isrow(rand(2,1,1)), %f);
assert_checkequal(isrow([]), %f);

// Strings
assert_checkequal(isrow(["s" "t" "u"]), %t);
assert_checkequal(isrow(["s" "t" "u"]'), %f);
assert_checkequal(isrow(["s" "t" "u";"s" "t" "u"]), %f);
assert_checkequal(isrow("s"), %t);
assert_checkequal(isrow(hypermat([1,1,3],["s";"s";"s"])), %t);
assert_checkequal(isrow(hypermat([2,1,1],["s";"s"])), %f);

// Polynomials
s=poly(0,"s");
assert_checkequal(isrow([1+s 2+s]), %t);
assert_checkequal(isrow([1+s 2+s]'), %f);
assert_checkequal(isrow([1+s 2+s; 3+s 4+s]), %f);
assert_checkequal(isrow(1+s), %t);
assert_checkequal(isrow(hypermat([1,1,3],[s;s;s])), %t);
assert_checkequal(isrow(hypermat([2,1,1],[s;s])), %f);

// Booleans
assert_checkequal(isrow([%t %f]), %t);
assert_checkequal(isrow([%t %f]'), %f);
assert_checkequal(isrow([%t %f; %f %t]), %f);
assert_checkequal(isrow(%t), %t);
assert_checkequal(isrow(hypermat([1,1,3],[%t;%t;%t])), %t);
assert_checkequal(isrow(hypermat([2,1,1],[%t;%t])), %f);

// Sparses
assert_checkequal(isrow(sparse([1 0 2])), %t);
assert_checkequal(isrow(sparse([1 0 2])'), %f);
assert_checkequal(isrow(sparse([1 0 2; 0 1 3])), %f);
assert_checkequal(isrow(sparse(0)), %t);

// Structures
clear s;
assert_checkequal(isrow(struct()), %f); // Empty structure
s.a = "test";
assert_checkequal(isrow(s), %t); // Scalar structure
clear s;
s(3,1).a = %pi;
assert_checkequal(isrow(s), %f); // Column structure
clear s;
s(1,3).a = %e;
assert_checkequal(isrow(s), %t); // Row structure
clear s;
s(1,1,3).a = %pi;
assert_checkequal(issquare(s), %f); // Row structure
clear s;
s(3,2).a = %z;
assert_checkequal(isrow(s), %f); // 2D structure array (not square)
clear s;
s(2,2,2).a = %s;
assert_checkequal(isrow(s), %f); // 3D structure array (cube)
clear s;
s(2,1,2).a = %s;
assert_checkequal(isrow(s), %f); // 3D structure array with singleton (square)

// Cells
assert_checkequal(isrow(cell()), %f); // Empty cell
a = cell(1);
a(1).entries = 1;
assert_checkequal(isrow(a), %t); // Scalar case
clear a;
a = cell(1,3);
a(1).entries = 1:3;
a(2).entries = 1:3;
a(3).entries = 1:3;
assert_checkequal(isrow(a), %t); // Row case
clear a;
a = cell(3,1);
a(1).entries = 1:3;
a(2).entries = 1:3;
a(3).entries = 1:3;
assert_checkequal(isrow(a), %f); // Column case
clear a;
a = cell(3,2);
a(1,1).entries = 1:3;
a(1,2).entries = 1:3;
a(2,1).entries = 1:3;
a(2,2).entries = 1:3;
a(3,2).entries = 1:3;
a(3,1).entries = 1:3;
assert_checkequal(isrow(a), %f); // Matrix case
clear a;
a = cell(2,2,2);
a(1,1,1).entries =1:3;
a(1,2,1).entries =1:3;
a(2,1,1).entries =1:3;
a(2,2,1).entries =1:3;
a(1,1,2).entries =1:3;
a(1,2,2).entries =1:3;
a(2,1,2).entries =1:3;
a(2,2,2).entries =1:3;
assert_checkequal(isrow(a), %f); // Cubic case
a = cell(2,1,2);
a(1,1,1).entries=1:3;
a(2,1,1).entries=1:3
a(1,1,2).entries=1:3;
a(2,1,2).entries=1:3;
assert_checkequal(isrow(a), %f); // Hypermatrix with singleton (square)

// Lists
assert_checkequal(isrow(list()), %f); // Empty list
l=list(1);
assert_checkequal(isrow(l), %t); // Scalar case
clear l;
l=list(1,"test");
assert_checkequal(isrow(l), %f); // Column case

// Error messages
errmsg1 = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"isrow", 1);
assert_checkerror("isrow()", errmsg1);
errmsg2 = msprintf(_("Wrong number of input arguments."));
assert_checkerror("isrow(1,2)", errmsg2);
errmsg3 = msprintf(_("Wrong number of output arguments.\n"));
assert_checkerror("[r,b]=isrow([1 2 3]);", errmsg3);
