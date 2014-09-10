// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

loadXcosLibs();

// Allocate a diagram and access its fields
scs_m = scicos_diagram()
scs_m.props
scs_m.objs
scs_m.version
scs_m.contrib

// Create a diagram containing 2 summation blocks and an empty link
blk  = BIGSOM_f("define");
blk2 = BIGSOM_f("define");
lnk  = scicos_link();

scs_m = scicos_diagram( objs=list(blk,blk2,lnk) )
scs_m.objs

// Link output port #1 of block #1 (ID #5 in this example) with input port #1 of block #2 (ID #7)
// The link ID is #10
l = scs_m.objs;
l(3).from = [1 1 0];
l(3).to = [2 1 1];
l(3).from
l(3).to

// Change the end of the link to input port #2 of block #2 (ID #8)
l(3).to = [2 2 1];
l(3).from
l(3).to

// Disconnect the source, the destination is also disconnected
l(3).from = [0 0 0];
l(3).from
l(3).to
