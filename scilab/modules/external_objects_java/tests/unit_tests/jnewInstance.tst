// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

jimport java.io.File;
jimport java.lang.System;

a = System.getProperty("java.io.tmpdir");
s = jnewInstance(File, a);
assert_checktrue(s.exists());

str = jnewInstance("java.lang.String", "Hello");
assert_checkequal(str.substring(1, 4),"ell");

jremove Date System s str b a;
