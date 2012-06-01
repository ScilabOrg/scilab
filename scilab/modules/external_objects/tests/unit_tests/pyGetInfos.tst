// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a=pyGetInfos();
nb=size(a,"*");

assert_checktrue(nb>0);
assert_checkerror("pyGetInfos(2)", [], 77);