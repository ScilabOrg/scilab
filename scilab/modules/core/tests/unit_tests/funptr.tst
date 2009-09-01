// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//================================================
// test funptr
//================================================
ptr = funptr('pwd');
if ptr <> 34031 then pause,end

ierr = execstr('ptr = funptr(1)','errcatch');
if ierr <> 999 then pause,end

ptr = funptr('FCT_NOT_IN_SCILAB');
if ptr <> 0 then pause,end

//================================================
