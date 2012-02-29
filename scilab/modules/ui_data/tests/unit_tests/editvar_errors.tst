// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// editvar unit tests

// <-- ENGLISH IMPOSED -->

refMsg = msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "editvar", 1, 4);
assert_checkerror("editvar();", refMsg);

refMsg = msprintf(_("%s: Wrong number of input argument(s): %d, %d or %d expected.\n"), "editvar", 1, 2, 4);
assert_checkerror("editvar(1,2,3);", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: A String expected.\n"), "editvar", 1);
assert_checkerror("editvar([]);", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: A String expected.\n"), "editvar", 1);
assert_checkerror("editvar(12);", refMsg);

clear __ghost
refMsg = msprintf(_("%s: Undefined variable: %s.\n"), "editvar", "__ghost");
assert_checkerror("editvar(""__ghost"");", refMsg);
