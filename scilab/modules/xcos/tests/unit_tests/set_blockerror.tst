// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ierr=execstr("set_blockerror(-1)","errcatch"); 
if ierr <> 999 then pause,end
ierr=execstr("set_blockerror(-2)","errcatch"); 
if ierr <> 999 then pause,end
ierr=execstr("set_blockerror(-3)","errcatch"); 
if ierr <> 999 then pause,end
ierr=execstr("set_blockerror(-16)","errcatch"); 
if ierr <> 999 then pause,end
