// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Jérôme Picard
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
//
// <-- TEST WITH XCOS -->
//
// <-- Non-regression test for bug 7770 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7770
//
// <-- Short Description -->
// It was not possible to compile a xcos diagram
// containing Modelica blocks

// open a Modelica demo
xcos(SCI + "/modules/xcos/demos/ModelicaBlocks/RLC_Modelica.xcos");

// compile the diagram by clicking on compile 
// in simulation menu
