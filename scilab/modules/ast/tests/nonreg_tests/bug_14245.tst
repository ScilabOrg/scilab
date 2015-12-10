// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14245 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14245
//
// <-- Short Description -->
// Problem in recursive extraction using list with struct

a=[1,2;3,4];
b=list(3,a);
c=5;

s=list();
s(1)=struct("a",a,"b",b,"c",c);
s(2)=2;

assert_checkequal(s(1).b(2)(1:3), [1 ; 3 ; 2]);
