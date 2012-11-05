// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a=sparse([%t %t %t; %t %t %t]);
b=matrix(a,1,-1);
assert_checkequal(size(a), [2,3]);
assert_checkequal(size(b), [1,6]);
