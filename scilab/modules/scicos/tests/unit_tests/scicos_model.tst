// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================+
//
// <-- CLI SHELL MODE -->
//

loadXcosLibs();

// Generate a default graphics list
myGraphics = scicos_graphics();
myModel = scicos_model();

p = funcprot();
funcprot(0);
mlist = scicos_new;
setfield = scicos_setfield;
funcprot(p);

// Allocate a specific block
o = scicos_block(graphics=myGraphics, model=myModel);

// Tune myModel and pass it to the block
myModel.sim = list("blk", 5);
myModel.state = [1; 2];
myModel.blocktype = "d";
myModel.nzcross = 3;
o.model = myModel;
