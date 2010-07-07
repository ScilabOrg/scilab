// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- TEST WITH XCOS -->
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// White-box test for the xcosPalAddBlock macro.

// Init
loadScicosLibs;
pal = xcosPal();
iconPath = SCI + "/modules/xcos/images/palettes/NPN.png";

// Test block argument
scs_m = SUM_f("define");
blockPath = TMPDIR + "/block.h5";
pal = xcosPalAddBlock(pal, "SUM_f")
pal = xcosPal();
pal = xcosPalAddBlock(pal, scs_m)
pal = xcosPal();
export_to_hdf5(blockPath, "scs_m");
pal = xcosPalAddBlock(pal, blockPath)
pal = xcosPal();

// Test pal_block_img argument
pal = xcosPalAddBlock(pal, blockPath, iconPath)
pal = xcosPal();
current = pwd();
cd(SCI);
pal = xcosPalAddBlock(pal, blockPath, "modules/xcos/images/palettes/NPN.pn?")
pal = xcosPal();
cd(current);

// Test style argument
myStyle = struct();
myStyle.block = [];
myStyle.image = iconPath;
pal = xcosPalAddBlock(pal, blockPath, style=myStyle)
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, style=iconPath)

