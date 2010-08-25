// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7366 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7366
//
// <-- Short Description -->
// There wasn't the possibility to have LaTeX in menus.

f=figure();
m=uimenu(f,"Label","$\int_0^\infty\mathrm{e}^{x^2}\,dx$");
uimenu(m,"Label","$\frac{\sqrt\pi}{2}$");
h=uicontrol(f,"style","pushbutton","string","$x^2$");

// You should see a menu with a menuitem and a pushbutton containing LaTeX images