// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 1326 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13226
//
// <-- Short Description -->
// Completion with accentued chars could lead to a a crash

createdir(TMPDIR+"/Téléchargements");
mputl("",TMPDIR+"/Téléchargements/foo.bar");
cd(TMPDIR);

// enter at prompt:
// atomsInstall("./Téléchargements/f
// and try to complete with <TAB>