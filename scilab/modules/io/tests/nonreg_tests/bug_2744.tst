// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 2744 -->
//
// <-- Short Description -->
// read cannot deal with NAN. 
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2744
//


ref = [%nan 1 %nan 2 %inf 3];
write(TMPDIR + '/matrix.dat', ref);
res = read(TMPDIR + '/matrix.dat',-1 ,size(ref,'*'));

if and(ref <> res) then pause,end

if ~isnan(res(1)) then pause,end
if ~isnan(res(3)) then pause,end
if ~isinf(res(5)) then pause,end