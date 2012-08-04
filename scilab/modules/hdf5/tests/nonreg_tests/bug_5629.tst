// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5629 and 5630 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5629
// http://bugzilla.scilab.org/show_bug.cgi?id=5630
//
// <-- Short Description -->
// 5629: export_to_hdf5 overwrites an empty directory without warning
// 5630: export_to_hdf5 on an existing directory with file in it mess with the permissions.

commonName = pathconvert(TMPDIR+"/bug_5629",%F);
myvar      = 1;

mkdir(commonName);
if ~isdir(commonName) then pause,end
if ~execstr("export_to_hdf5(commonName,""myvar"")","errcatch") then pause, end
if ~isdir(commonName) then pause,end
if isfile(commonName) then pause,end
