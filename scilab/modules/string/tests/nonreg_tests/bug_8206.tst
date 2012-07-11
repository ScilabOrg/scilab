// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8206 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8206
//
// <-- Short Description -->
// isnum did not manage blanks
//

a = ["-4.5", ""; "", "-5.6"];
ref = [%T %F; %F %T];
r = isnum(a);
if ~or(r == ref) then pause, end

if isnum("%inf") <> %t then pause, end
if isnum("%nan") <> %t then pause, end

b = ["-4.5", "", "%nan", "%f", "%i", "%pi"];
r = isnum(b);
ref = [%T %F %T %F %T %T];
if ~or(r == ref) then pause, end

C = 1;
c = ["-4.5", "blabla"; "C", "-5.6"];
r = isnum(a);
ref = [%T %F; %T %T];
if ~or(r == ref) then pause, end
