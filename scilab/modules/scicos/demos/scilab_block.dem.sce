// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

thispath = get_absolute_file_path("scilab_block.dem.sce");
// scicos_demostration(thispath+"/Scilab_Block.cosf");

//Open the cosf file with xcos
xcos(thispath+"/Scilab_Block.cosf");

clear thispath;
