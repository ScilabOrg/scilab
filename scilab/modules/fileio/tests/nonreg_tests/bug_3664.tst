// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3664 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3664
//
// <-- Short Description -->
//  an error occurs when a change directory operation implies a directory name with french accent letter ('�' for example).

curdir = pwd();
mkdir(TMPDIR,'c�d���');
if cd(TMPDIR + filesep() + 'c�d���') <> TMPDIR + filesep() + 'c�d���' then pause,end
cd(curdir);

if chdir(TMPDIR + filesep() + 'c�d���') <> %T then pause,end