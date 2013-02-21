// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

ilib_verbose(0);
ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
if ierr<>0 then pause, end

currentline = 'cd /t';
r = completeline(currentline,'/tmp',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'cd /tmp' then pause,end

r = completeline('cd /home/sy','sylvestre',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'cd /home/sylvestre' then pause,end

r = completeline('cd d:\GIT-scilab-','d:\GIT-scilab-branch',getpartlevel(currentline),getfilepartlevel(currentline),%t);
if r <> 'cd d:\GIT-scilab-branch' then pause,end

currentline = 'cd C:\ProgramD';
r = completeline(currentline,'ProgramData',getpartlevel(currentline),getfilepartlevel(currentline),%t);
if r <> 'cd C:\ProgramData' then pause,end

currentline = 'cd C:\Program F';
r = completeline(currentline,'Program Files',getpartlevel(currentline),getfilepartlevel(currentline),%t);
if r <> 'cd C:\Program Files' then pause,end

currentline = 'cd C:\Windows\Sys';
r = completeline(currentline,'System32',getpartlevel(currentline),getfilepartlevel(currentline),%t);
if r <> 'cd C:\Windows\System32' then pause,end

currentline = 'ho';
r = completeline(currentline,'home',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'home' then pause,end

currentline = 'TMP';
r = completeline(currentline,'TMPDIR',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'TMPDIR' then pause,end

currentline = 'disp(TMP';
r = completeline(currentline,'TMPDIR',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'disp(TMPDIR' then pause,end

currentline = '1 + ab';
r = completeline(currentline,'abs',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> '1 + abs' then pause,end

currentline = 'tmpStr = ""TMP = "" + TMP';
r = completeline(currentline,'TMPDIR',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'tmpStr = ""TMP = "" + TMPDIR' then pause,end

currentline = 'mytab = ab';
r = completeline(currentline,'abs',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'mytab = abs' then pause,end

currentline = 't.fie';
r = completeline(currentline,'field1',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 't.field1' then pause,end

currentline = 't.field1 = t.fie';
r = completeline(currentline,'field2',getfilepartlevel(currentline),getpartlevel(currentline),%t);
