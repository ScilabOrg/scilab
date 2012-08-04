// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- CLI SHELL MODE -->

mclose('all');

// Check that mclose('all') closes all files (except stdin and stdout)
f1 = mopen(TMPDIR + filesep() + "mclose1.txt","w");
f2 = mopen(TMPDIR + filesep() + "mclose2.txt","w");
mclose("all");

if size(file())<>2 then pause; end
