// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->

// Load and run a diagram that calls a Scilab macro (tkscaleblk.sci)
exec("SCI/modules/scicos/tests/unit_tests/bug_8348.cosf", -1);
cpr = scicos_simulate(scs_m);

// Check the sensitive value of the continuous state
assert_checkalmostequal(cpr(2)(2)(9)(4), 5.49956686330299);
