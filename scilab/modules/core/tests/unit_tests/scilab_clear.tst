// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit test for Scilab clear

x=[1,2,3];
clear x;
assert_checktrue(exists('x')==0);
x=[1,2,3];y='afs';
clear x y;
assert_checktrue(exists('x')|exists('y')==0);
X=[1,2,3];Y='afs';
clear X Y;
assert_checktrue(exists('X')|exists('Y')==0);
