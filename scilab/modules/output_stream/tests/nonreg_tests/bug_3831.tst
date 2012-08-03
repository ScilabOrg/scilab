// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3831 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3831
//
// <-- Short Description -->
// Display bug of ascii() in SciLab 5.x

refASCIICODE = [84  114  97  110  115  109  105  115  115  105  111  110];
refSTRING = 'Transmission';
res = ascii(refASCIICODE);
if length(res) <> length(refSTRING) then pause,end
if refSTRING <> res then pause,end

refASCIICODE2 = [84  114  97  110  115  0  105  115  115  105  111  110];
refSTRING2 = 'Trans' + ascii(0) + 'ission';
// no ; to display result
res = ascii(refASCIICODE2)
if length(res) <> length(refSTRING2) then pause,end
if refSTRING2 <> res then pause,end



