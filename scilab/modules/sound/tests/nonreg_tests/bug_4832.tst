// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4832 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4832
//
// <-- Short Description -->
// waveread can not detect RIFF with smpl chunck
// a non explicit error is returned

filename = SCI + '/modules/sound/tests/nonreg_tests/bug_4832.wav';
ierr = execstr('wavread(filename)','errcatch');
if ierr<> 10000 then pause,end

