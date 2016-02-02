// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- TEST WITH GRAPHIC -->

// Simple example with Checked menu
h = uimenu("parent", gcf(), "label", "parent");
h1 = uimenu("parent", h, "label", "child1", "checked", "off");
h2 = uimenu("parent", h, "label", "child2");

// Checked menu which becomes a parent menu
h = uimenu("parent", gcf(), "label", "parent");
h1 = uimenu("parent", h, "label", "child1");
h2 = uimenu("parent", h, "label", "child2", "checked", "off");
h3 = uimenu("parent", h1, "label", "subchild11", "checked", "off");
h4 = uimenu("parent", h2, "label", "subchild21", "checked", "on");

h = uimenu("label","test");
assert_checkequal(h.parent, gcf());
assert_checkequal(h.enable, "on");
assert_checkequal(h.label, "test");
assert_checkequal(h.visible, "on");
assert_checkequal(h.callback, "");
assert_checkequal(h.callback_type, 0);
assert_checkequal(h.checked, "off");
assert_checkequal(h.tag, "");

f=scf();
h = uimenu("Parent", f, ..
    "Enable","off",..
    "Label", "test", ..
    "Visible","off",..
    "Callback","disp(1)",..
    "Checked", "on", ..
    "Tag", "hello");
assert_checkequal(h.parent, f);
assert_checkequal(h.enable, "off");
assert_checkequal(h.label, "test");
assert_checkequal(h.visible, "off");
assert_checkequal(h.callback, "disp(1)");
assert_checkequal(h.callback_type, 0);
assert_checkequal(h.checked, "on");
assert_checkequal(h.tag, "hello");

// Add a menu in default figure
f=gdf();
m=uimenu(f, "label", "windows");
m1=uimenu(m, "label", "operations");
f=scf(50);
assert_checkequal(f.children(2).Label, "windows");
assert_checkequal(f.children(2).children.Label, "operations");
