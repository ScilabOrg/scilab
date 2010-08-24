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
iconPath = SCI + "/modules/xcos/images/palettes/NPN.png";

// Test block argument
scs_m = SUM_f("define");
blockPath = TMPDIR + "/block.h5";
pal = xcosPal();
pal = xcosPalAddBlock(pal, "SUM_f");

expectedResult = ["SUM_f" msprintf("%s/SUM_f.h5", TMPDIR) msprintf("%s/SUM_f.gif", TMPDIR) msprintf("noLabel=1;image=file:%s/SUM_f.svg;", TMPDIR)];
result = [pal.blockNames(1) pal.blocks(1) pal.icons(1) pal.style(1)];
if or(expectedResult <> result) then pause, end

pal = xcosPal();
pal = xcosPalAddBlock(pal, scs_m);

expectedResult = ["SUM_f" msprintf("%s/SUM_f.h5", TMPDIR) msprintf("%s/SUM_f.gif", TMPDIR) msprintf("noLabel=1;image=file:%s/SUM_f.svg;", TMPDIR)];
result = [pal.blockNames(1) pal.blocks(1) pal.icons(1) pal.style(1)];
if or(expectedResult <> result) then pause, end

pal = xcosPal();
export_to_hdf5(blockPath, "scs_m");
pal = xcosPalAddBlock(pal, blockPath);

expectedResult = ["SUM_f" blockPath msprintf("%s/SUM_f.gif", TMPDIR) msprintf("noLabel=1;image=file:%s/SUM_f.svg;", TMPDIR)];
result = [pal.blockNames(1) pal.blocks(1) pal.icons(1) pal.style(1)];
if or(expectedResult <> result) then pause, end

pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, [], []);
expectedResult = ["SUM_f" blockPath msprintf("%s/SUM_f.gif", TMPDIR) msprintf("noLabel=1;image=file:%s/SUM_f.svg;", TMPDIR)];
result = [pal.blockNames(1) pal.blocks(1) pal.icons(1) pal.style(1)];
if or(expectedResult <> result) then pause, end


// Test pal_block_img argument
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, iconPath);
current = pwd();
cd(SCI);
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, "modules/xcos/images/palettes/NPN.png");

expectedResult = ["SUM_f" blockPath msprintf("%s/modules/xcos/images/palettes/NPN.png", SCI) msprintf("noLabel=1;image=file:%s/SUM_f.svg;", TMPDIR)];
result = [pal.blockNames(1) pal.blocks(1) pal.icons(1) pal.style(1)];
if or(expectedResult <> result) then pause, end

cd(current);

// Test style argument
myStyle = struct();
myStyle.block = [];
myStyle.image = "file:" + iconPath;
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, [], myStyle);

expectedResult = ["SUM_f" blockPath msprintf("%s/SUM_f.gif", TMPDIR)  msprintf("block;image=file:%s/modules/xcos/images/palettes/NPN.png;", SCI)];
result = [pal.blockNames(1) pal.blocks(1) pal.icons(1) pal.style(1)];
if or(expectedResult <> result) then pause, end

pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath, [], iconPath);

expectedResult = ["SUM_f" blockPath msprintf("%s/SUM_f.gif", TMPDIR)  msprintf("noLabel=1;image=file:%s/modules/xcos/images/palettes/NPN.png;", SCI)];
result = [pal.blockNames(1) pal.blocks(1) pal.icons(1) pal.style(1)];
if or(expectedResult <> result) then pause, end

// Test pal_block_img argument and style argument simultaneously
// an image for the block in the diagram and an image for the icon of the block
pal = xcosPal();
pal = xcosPalAddBlock(pal, blockPath,iconPath,"modules/xcos/images/palettes/NPN.png");

expectedResult = ["SUM_f" blockPath iconPath  msprintf("noLabel=1;image=file:%s/modules/xcos/images/palettes/NPN.png;", SCI)];
result = [pal.blockNames(1) pal.blocks(1) pal.icons(1) pal.style(1)];
if or(expectedResult <> result) then pause, end

