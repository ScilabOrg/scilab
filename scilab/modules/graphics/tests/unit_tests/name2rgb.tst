// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Juergen Koch
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Unit-test for name2rgb -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/
//
// <-- Short Description -->
// Tests name2rgb
// 
assert_checkequal(name2rgb("red"), [255 0 0])
assert_checkequal(name2rgb("nocolor"), [])
assert_checkequal(name2rgb(["red" "green"]), [255 0 0;0 255 0])
assert_checkequal(name2rgb(["red";"green"]), [255 0 0;0 255 0])
assert_checkequal(name2rgb(["red" "green";"blue" "black"]), [255 0 0;0 0 255;0 255 0;0 0 0])
assert_checkequal(name2rgb(["red" "green";"blue" "nocolor"]), [])
