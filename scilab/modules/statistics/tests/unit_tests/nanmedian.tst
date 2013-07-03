// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Short Description -->
//
// Unit test for nanmedian function

assert_checkequal(nanmedian([1 2 3 %nan; 4 5 6 %nan],'c'), [2;5]);
assert_checkequal(nanmedian([1 2 3; 4 5 6; %nan %nan %nan],'r'), [2.5 3.5 4.5]);

assert_checkequal(nanmedian([1, 2, 3, %nan]','r'), 2);
assert_checkequal(nanmedian([1, 2, 3; %nan %nan %nan],'r'), [1 2 3]);

assert_checkequal(nanmedian([1, 2, 3; %nan %nan %nan]','c'),[1 ; 2 ; 3]);
assert_checkequal(nanmedian([1, 2, 3, %nan],'c'), 2);

assert_checkequal(nanmedian([1, 2, 3, 4; %nan %nan %nan %nan]','c'), [1 ; 2 ; 3 ; 4]);
assert_checkequal(nanmedian([1, 2, 3, 4, %nan],'c'), 2.5);

assert_checkequal(nanmedian([1, 2, 3, 4; %nan %nan %nan %nan],'r'), [1 2 3 4]);
assert_checkequal(nanmedian([1, 2, 3, 4, %nan]','r'), 2.5);
