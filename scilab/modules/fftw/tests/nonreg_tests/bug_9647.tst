// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9647 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9647
//
// <-- Short Description -->
// On Scilab startup, the variable fft was declared by default while it should not.
//

if %fftw then
    assert_checkequal(isdef("fft"), %T);
    assert_checkequal(funptr("fft"), funptr("fftw"));
else
    assert_checkequal(isdef("fft"), %F);
end
