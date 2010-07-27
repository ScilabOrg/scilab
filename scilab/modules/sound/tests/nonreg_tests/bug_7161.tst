// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7161 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7161
//
// <-- Short Description -->
// wavread failed to load some .wav files with error 44

filename = SCI + '/modules/sound/tests/nonreg_tests/bug_7161.wav';
ierr = execstr('[Ip, Fs, bits] = wavread(filename);', 'errcatch');
if ierr <> 0 then pause, end
sIp = size(Ip);
if sIp(1) <> 1 then pause, end
if sIp(2) <> 65536 then pause, end
if Fs <> 8000 then pause, end
if bits <> 16 then pause, end
