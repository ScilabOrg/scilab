// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- JVM MANDATORY -->
//
// <-- Non-regression test for bug 4834 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4834
//
// <-- Short Description -->
// When called with more than 11 items displaytree did not open a window and Scilab reported a java exception.

l1 = uiCreateNode("CONST_m","default","");
l2 = uiCreateNode("GENSQR_f","default","");
l3 = uiCreateNode("RAMP","default","");
l4 = uiCreateNode("RAND_m","default","");
l5 = uiCreateNode("RFILE_f","default","");
l6 = uiCreateNode("CLKINV_f","default","");
l7 = uiCreateNode("CURV_f","default","");
l8 = uiCreateNode("INIMPL_f","default","");
l9 = uiCreateNode("SAWTOOTH_f","default","");
l10 = uiCreateNode("READAU_f","default","");
l11 = uiCreateNode("WRITEAU_f","default","");

root1 = uiCreateNode("Sources");

tree = uiCreateTree(root1,l1,l2,l3,l4,l5,l6,l7,l8,l9,l10,l11);

uiDisplayTree(tree)
