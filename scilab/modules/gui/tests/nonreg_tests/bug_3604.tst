// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 3604 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3604
//
// <-- Short Description -->
//    The small script attached to this bug make Scilab crash.
//    ==> Problem with getting Frame position

nom_fig = "test IHM";
fig = figure(1);

set(fig,"Figure_name",nom_fig);
set(fig,"Position",[400 350 500 300]);

fond = uicontrol(fig,"Style","frame","Units","normalized","Position",[0.2 0.2 0.6 0.6],"BackgroundColor",[0.7 0.7 0.7]);
legende_1 = uicontrol(fond,"Style","text","Units","normalized","Position",[0.1 0.1 0.8 0.8],"String","Welcome SCILAB 5.0","BackgroundColor",[0.7 0.7 0.7],"Fontweight","bold");

