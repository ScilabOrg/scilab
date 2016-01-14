// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - DIGITEO - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->


// line break
colvect(1,1) = 1;
colvect(2,1) = -2;
linevect = colvect';


a=[1; ...
-2];
assert_checkequal(a,  colvect);
a=[1;...
-2];
assert_checkequal(a,  colvect);
a=[1; ...
 -2];
assert_checkequal(a,  colvect);
a=[1;...
 -2];
assert_checkequal(a,  colvect);
a=[1; ...
- 2];
assert_checkequal(a,  colvect);
a=[1;...
- 2];
assert_checkequal(a,  colvect);

a=[1 ...
-2];
assert_checkequal(a,  linevect);
a=[1...
-2];
assert_checkequal(a,  -1);
a=[1 ...
 -2];
assert_checkequal(a,  linevect);
a=[1...
 -2];
assert_checkequal(a,  linevect);
a=[1 ...
- 2];
assert_checkequal(a,  -1);
a=[1...
- 2];
assert_checkequal(a,  -1);

a=[1; ...//comment
-2];
assert_checkequal(a,  colvect);
a=[1;...//comment
-2];
assert_checkequal(a,  colvect);
a=[1; ...//comment
 -2];
assert_checkequal(a,  colvect);
a=[1;...//comment
 -2];
assert_checkequal(a,  colvect);
a=[1; ...//comment
- 2];
assert_checkequal(a,  colvect);
a=[1;...//comment
- 2];
assert_checkequal(a,  colvect);

a=[1 ...//comment
-2];
assert_checkequal(a,  linevect);
a=[1...//comment
-2];
assert_checkequal(a,  -1);
a=[1 ...//comment
 -2];
assert_checkequal(a,  linevect);
a=[1...//comment
 -2];
assert_checkequal(a,  linevect);
a=[1 ...//comment
- 2];
assert_checkequal(a,  -1);
a=[1...//comment
- 2];
assert_checkequal(a,  -1);

a=[1 ...
-2 ];
assert_checkequal(a,  linevect);
a=[1...
-2 ];
assert_checkequal(a,  -1);
a=[1 ...
 -2 ];
assert_checkequal(a,  linevect);
a=[1...
 -2 ];
assert_checkequal(a,  linevect);
a=[1 ...
- 2 ];
assert_checkequal(a,  -1);
a=[1...
- 2 ];
assert_checkequal(a,  -1);
