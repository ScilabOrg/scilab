// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// Unit test for macro legend()
//

// check legends order
scf();

plot([1,2], [1,10], "b");
h1 = gce();
h1 = h1.children(1);

plot([1,2], [1,20], "r");
h2 = gce();
h2 = h2.children(1);

plot([1,2], [1,30], "g");
h3 = gce();
h3 = h3.children(1);

labels = ["1"; "2"; "3"];

c = legend(labels);
assert_checkequal(c.text, labels);

c = legend([h1, h2, h3], labels);
assert_checkequal(c.text, labels);
