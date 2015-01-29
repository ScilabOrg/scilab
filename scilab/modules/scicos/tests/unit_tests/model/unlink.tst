// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- XCOS TEST -->

b1 = scicos_block();
b2 = scicos_block();
l1 = scicos_link( from=[1 1 0],to=[2 1 1] );
l2 = scicos_link();
l3 = scicos_link( from=[1 2 0],to=[2 2 1] );
scs_m = scicos_diagram( objs=list(b1, b2, l1, l2, l3) );

// Check that each block's 2 ports are linked to l1 (resp l3)
assert_checkequal(scs_m.objs(1).graphics.pout, [3; 5]);
assert_checkequal(scs_m.objs(2).graphics.pin,  [3; 5]);

// Check that it is impossible to link to an already linked port (error message displayed)
scs_m.objs(4).from = [1 1 0];
assert_checkequal(scs_m.objs(1).graphics.pout, [3; 5]);
assert_checkequal(scs_m.objs(2).graphics.pin,  [3; 5]);
scs_m.objs(4).from = [1 2 0];
assert_checkequal(scs_m.objs(1).graphics.pout, [3; 5]);
assert_checkequal(scs_m.objs(2).graphics.pin,  [3; 5]);

scs_m.objs(4).to = [2 1 1];
assert_checkequal(scs_m.objs(1).graphics.pout, [3; 5]);
assert_checkequal(scs_m.objs(2).graphics.pin,  [3; 5]);
scs_m.objs(4).to = [2 2 1];
assert_checkequal(scs_m.objs(1).graphics.pout, [3; 5]);
assert_checkequal(scs_m.objs(2).graphics.pin,  [3; 5]);


// Check that all model items are freed
scicos_log("TRACE");
clear
