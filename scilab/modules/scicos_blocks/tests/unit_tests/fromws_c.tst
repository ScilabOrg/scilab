// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
// <-- XCOS TEST -->

exec("SCI/modules/scicos_blocks/tests/unit_tests/fromws_c.cosf", -1);

V.values = ones(10, 1);
// Intentionally giving V.values 10 elements and V.time 11 elements to make scicos_simulate fail
V.time = (0:.1:1)';

refMsg = _("Initialization problem:");
assert_checkerror("scicos_simulate(scs_m);", refMsg);

// Reduce V.time to 10 elements, to square with V.values size
V.time = V.time(1:$-1, 1);

cpr = scicos_simulate(scs_m);
