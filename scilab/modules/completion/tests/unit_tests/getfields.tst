// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

ilib_verbose(0);
ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
if ierr<>0 then pause, end

a=mlist(['TYPE' 'abcd' 'efgh' 'abce' 'ab']);
r = getfields('a.');
if ~and(r==['ab'; 'abcd'; 'abce'; 'efgh']) then pause,end;

r = getfields('a.a');
if ~and(r==['ab'; 'abcd'; 'abce']) then pause,end;

r = getfields('foo;a.abc');
if ~and(r==['abcd'; 'abce']) then pause,end;

r = getfields('b.');
if ~isempty(r) then pause,end