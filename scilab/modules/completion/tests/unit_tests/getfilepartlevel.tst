// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(SCI+"/modules/completion/tests/utilities/loader.sce","errcatch",-1);
if ierr<>0 then pause, end

r = getfilepartlevel('cd c:\Program Files\scilab-5.1');
if r <>'c:\Program Files\scilab-5.1' then pause,end;

r = getfilepartlevel('cd(""c:\Program Files\scilab-5.1');
if r <>'c:\Program Files\scilab-5.1' then pause,end;

r = getfilepartlevel('cd(''c:\Program Files\scilab-5.1');
if r <>'c:\Program Files\scilab-5.1' then pause,end;

r = getfilepartlevel('cd /home/scilabs/scilab-5.1');
if r <>'/home/scilabs/scilab-5.1' then pause,end;

r = getfilepartlevel('cd(""/home/scilabs/scilab-5.1');
if r <>'/home/scilabs/scilab-5.1' then pause,end;

r = getfilepartlevel('cd(''/home/scilabs/scilab-5.1');
if r <>'/home/scilabs/scilab-5.1' then pause,end;

r = getfilepartlevel('cd ../');
if r <>'../' then pause,end;

r = getfilepartlevel('cd ~/');
if r <>'~/' then pause,end;

r = getfilepartlevel('cd ~');
if r <>'~' then pause,end;

r = getfilepartlevel('cd ');
if r <>'' then pause,end;
