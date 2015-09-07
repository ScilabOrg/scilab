//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1772 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1772
//
// <-- Short Description -->
//    There is a problem while inserting an object of type list in an undefined struct
//    field

S.x=1;
if execstr("S.y.z=list();","errcatch") <> 0 then pause,end
if S.y.z<>list() then pause,end
