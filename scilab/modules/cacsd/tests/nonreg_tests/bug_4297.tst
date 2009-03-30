// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4297 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4297
//
// <-- Short Description -->
//     A spelling mistake was causing in some cases with the pfss function
W=ssrand(1,1,6);
if execstr("pfss(W,10);","errcatch")<>0 then pause,end




