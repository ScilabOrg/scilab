// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

thispath = get_absolute_file_path("rlc_circuit.dem.sce");

// electrical blocks are simulated with Modelica
xcos(thispath+"../ModelicaBlocks/RLC_Modelica.xcos");
clear thispath;
