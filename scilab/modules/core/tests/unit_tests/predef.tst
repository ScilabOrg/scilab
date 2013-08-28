// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- CIL SHELL MODE -->
//

N = predef();
a = 1;
predef("all");

ierr = execstr('clear a','errcatch');
if ierr <> 13 then pause,end

ierr = execstr('predef(''clear'')','errcatch');
if ierr <> 0 then pause,end

ierr = execstr('clear','errcatch');
if ierr <> 0 then pause,end

ierr = execstr('predef(N)','errcatch');
if ierr <> 0 then pause,end

if predef() <> N then pause,end

//-------------------------------------------

// Error checks
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A scalar or a string expected.\n"), "predef", 1);
assert_checkerror("predef(%t);", refMsg);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: An integer expected.\n"), "predef", 1);
assert_checkerror("predef(1.5);", refMsg);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "predef", 1);
assert_checkerror("predef([1 2]);", refMsg);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: ''%s'', ''%s'' or ''%s'' expected.\n"), "predef", 1, "clear", "all", "names");
assert_checkerror("predef(''g'');", refMsg);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "predef", 1);
assert_checkerror("predef([''g'' ''h'']);", refMsg);
