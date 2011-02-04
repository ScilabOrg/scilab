// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ierr = execstr('a = javalibrarypath(2);','errcatch');
if ierr <> 999 then pause,end

result = javalibrarypath();
if size(result,"*") < 1 then pause, end

// Does not return anything
ierr = execstr('javalibrarypath(TMPDIR);;','errcatch');
if ierr <> 0 then pause,end
