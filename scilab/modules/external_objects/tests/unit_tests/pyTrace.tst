// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

logFile=TMPDIR+"/foo.log";
pyTrace(logFile)
a = pyWrap(1:10);
b = a + (20:30);

pyRemove a b

// Stop logging
pyTrace();

assert_checktrue(isfile(logFile));

assert_checkerror("pyTrace(2)", [], 999);
