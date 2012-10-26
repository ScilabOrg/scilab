// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 11554 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11554
//
// <-- Short Description -->
// quit report an error after loading xcos_toolbox_skeleton.

// avoid building the xcos toolbox skeleton by calling xcosPalAdd(...)


function [x,y,typ]=MY_BLOCK(job,arg1,arg2)
    x=[];y=[];typ=[];
    select job
    case 'set' then
        x=arg1;
    case 'define' then
        model=scicos_model()
        model.sim=list('my_block',4)
        exprs=''
        gr_i=''
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
block_img = SCI + "/modules/xcos/images/blocks/RAMP.svg";
pal_block_img = SCI + "/modules/xcos/images/palettes/RAMP.png";



pal = xcosPal("My palette");
pal = xcosPalAddBlock(pal, 'MY_BLOCK', pal_block_img, block_img);
xcosPalAdd(pal);

quit();
