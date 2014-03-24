// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

res = x_mdialog(["Message";...
"(edit the boxes below --> output: edited boxes)"],...
["first row";"second row";"etc..."],...
["10";"20";"30"]);

disp(_("Values entered: "+res(1)+" "+res(2)+" "+res(3)));

n = 5;
m = 4;
mat = rand(n, m);
row = "row";
labelv = row(ones(1, n))+string(1:n);
col = "col";
labelh = col(ones(1, m))+string(1:m);
new = evstr(x_mdialog("Matrix to edit", labelv, labelh, string(mat)))

