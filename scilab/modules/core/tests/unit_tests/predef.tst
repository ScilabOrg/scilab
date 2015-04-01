// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//

a = 1;
predef("all");

ierr = execstr('clear a','errcatch');
assert_checktrue(ierr <> 0);

ierr = execstr('predef(''clear'')','errcatch');
assert_checktrue(ierr == 0);

ierr = execstr('clear','errcatch');
assert_checktrue(ierr == 0);

assert_checktrue(predef() == 0);

