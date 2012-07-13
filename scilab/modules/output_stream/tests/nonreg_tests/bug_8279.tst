// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 8279 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8279
//
// <-- Short Description -->
// error(144) returned a unexpected message.
//

if execstr("error(144);", "errcatch") <> 144 then pause, end
[strerror, ierr] = lasterror();
if ierr <> 144 then pause, end
// strerror have no '\n' 
if strerror <> strsubst(_("Undefined operation for the given operands.\n"), "\n", "") then pause, end
