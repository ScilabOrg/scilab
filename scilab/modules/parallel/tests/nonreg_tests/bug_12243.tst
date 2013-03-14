// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - S/E - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 12243 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12243
//
// <-- Short Description -->
// parellel_run did not run all calculations on 8 core processors

lines(0);
function a=g(arg1)
 a=arg1*arg1;
endfunction
res=parallel_run(1:24, g);
assert_checkequal(res, [1,4,9,16,25,36,49,64,81,100,121,144,169,196,225,256,289,324,361,400,441,484,529,576]);
assert_checkequal(size(res),[1,24]);
