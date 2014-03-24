// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

res = messagebox(["messagebox: Simple example of message";
"First row of comments";
"Second row";
"etc...";], _("Title"), "info", _(["Button 1" "Button 2"]), "modal");

if res == 1 then
    disp(_("You clicked on Button 1"));
else
    disp(_("You clicked on Button 2"));
end
