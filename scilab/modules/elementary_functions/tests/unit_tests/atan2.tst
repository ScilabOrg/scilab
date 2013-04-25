// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

assert_checkequal(atan2([]),[]);

x=[1,%i,-1,%i]
assert_checkalmostequal(atan2(imag(x), real(x)), [0 1.5707963 3.1415927 1.5707963]);

assert_checkalmostequal(atan2(0, -1), 3.1415927);

assert_checkalmostequal(atan2(-%eps, -1), -3.1415927);
