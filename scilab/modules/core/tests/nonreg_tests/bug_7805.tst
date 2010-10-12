// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7805 -->
//
// <-- JVM NOT MANDATORY -->
//
// <-- INTERACTIVE TEST -->

// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7805
//
// <-- Short Description -->
// clear called inside an exec while in pause may have unexpected results
 
//copy an paste the following instructions in the Scilab Console.
mputl("clear;",TMPDIR+"/bug7805.sce");
a=1
pause
b=2
pause
c=3
exec(TMPDIR+"/bug7805.sce");
if exists("c")==1 then pause,end
if exists("b")==0 then pause,end
if exists("a")==0 then pause,end
