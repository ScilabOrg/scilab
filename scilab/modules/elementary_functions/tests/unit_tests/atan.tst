// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

assert_checkequal(atan([]), []);

assert_checkalmostequal(atan(-%eps+2*%pi), -1.5707963 + 0.5493061*%i);

assert_checkalmostequal(atan(+%eps+2*%pi),  1.5707963 + 0.5493061*%i);

assert_checkalmostequal(atan(-%eps-2*%pi), -1.5707963 - 0.5493061*%i);

assert_checkalmostequal(atan(+%eps-2*%pi),  1.5707963 - 0.5493061*%i);
