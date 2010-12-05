// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Test LaTeX and MathML to render text or ticks

plot2d();
a = gca();

// LaTeX with ticks labels
a.renderer = "latex";

// LaTeX with the title label
a.title.renderer = "latex";
xtitle("\text{Title with }\JLaTeXMath");

// LaTeX with a text
xstring(3, 0, "\frac{\pi^2}{6}");
t = get("hdl");
t.renderer = "latex";
t.font_size = 8;
t.font_angle = 30

// MathML with a text
xstring(3, 0, "<mover><mn>2</mn><mn>3</mn></mover>");
t=get("hdl");
t.renderer = "mathml";
t.font_size = 8;
t.font_angle = -30;

// LaTeX with a legend
l = legend(["\sum" "\int" "\prod"]);
l.renderer = "latex";

// We test now the export
xs2pdf(0, home + "/test_export.pdf");
