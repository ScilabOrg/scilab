// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3051 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3051
//
// <-- Short Description -->
// 

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

exec('SCI/modules/completion/tests/utilities/build_primitives.sce',-1);
exec('SCI/modules/completion/tests/utilities/loader.sce',-1);

// gethi<Tab>
currentline = 'gethi';

ref = ['gethistory';'gethistoryfile'];
r = completion(currentline);
if ~and(r == ref) then pause,end

if getcommonpart(r) <> 'gethistory' then pause,end
                    
cd SCI/contrib/;
//cd toolbo[TAB]
currentline = 'cd toolbo';
r = getfilepartlevel(currentline);
if r <> 'toolbo' then pause,end;

r2 = completion(r,'files');
if r2 <> 'toolbox_skeleton' + filesep() then pause,end

r = completeline(currentline,r2,getpartlevel(currentline),getfilepartlevel(currentline),%t);
if r <> 'cd toolbox_skeleton' + filesep() then pause,end
