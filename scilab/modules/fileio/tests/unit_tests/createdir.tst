// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

chdir(TMPDIR);

warning("off");
a = createdir("dir_азеазея");
if(a <> %T) then pause, end

b = cd(TMPDIR + filesep() + "dir_азеазея");
if(b <> (TMPDIR + filesep() + "dir_азеазея")) then pause, end

