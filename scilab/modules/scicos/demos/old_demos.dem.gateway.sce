// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

demopath = get_absolute_file_path("old_demos.dem.gateway.sce");

subdemolist = ["Old Gain Block"                              , "old_gain_block.dem.sce"  ];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;