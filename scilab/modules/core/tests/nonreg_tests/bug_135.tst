// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 135 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=135
//
// <-- Short Description -->
//    Il semble que l'affectation directe dans un element de vecteur ne soit pas possible!?

a = [];
if execstr("[a(1) a(2)]=size([1,2]);", "errcatch")<>0 then pause,end
