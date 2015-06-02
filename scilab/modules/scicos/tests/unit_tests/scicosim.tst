// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "scicosim", 6);
assert_checkerror("scicosim()", refMsg);
