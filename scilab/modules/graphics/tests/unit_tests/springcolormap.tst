// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

assert_checkerror("springcolormap(0,1,2,3)", "%s: Wrong number of input argument(s): %d expected.\n", 10000, "springcolormap", 1);

assert_checkerror("springcolormap(%t)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "springcolormap", 1);

assert_checkerror("springcolormap(%i)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "springcolormap", 1);

assert_checkerror("springcolormap([0 1 2 3])", "%s: Wrong size for input argument #%d: a real scalar expected.\n", 10000, "springcolormap", 1);

assert_checkequal(springcolormap(0), []);

assert_checkequal(springcolormap(1), [1 0 1]);

assert_checkequal(springcolormap(2), [1 0 1;1 1 0]);

assert_checkequal(springcolormap(3), [1 0 1;1 0.5 0.5;1 1 0]);
