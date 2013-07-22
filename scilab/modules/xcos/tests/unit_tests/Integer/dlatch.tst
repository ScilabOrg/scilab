// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Integer/dlatch.zcos"));

// Redefining messagebox() to avoid popup from c_pass2.sci
prot = funcprot();
funcprot(0);
function messagebox(msg, modal, msg_title)
    disp(msg);
endfunction
funcprot(prot);

try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end

assert_checktrue(A.values(1:7)   == 1);
assert_checktrue(A.values(8:31)  == 0);
assert_checktrue(A.values(32:46) == 1);
assert_checktrue(A.values(47:72) == 0);
