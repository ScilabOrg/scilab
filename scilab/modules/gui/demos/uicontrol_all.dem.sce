// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

f = figure("figure_size", [800 600], "dockable", "off", "layout", "border", "visible", "off", ...
"default_axes", "off");

tabPane = uicontrol(f, "style", "tab", ...
"constraints", createConstraints("border", "center"));

allStyles = ["pushbutton", "radiobutton", "checkbox", "slider", "frame"]

for style = allStyles
    demoFrame = uicontrol(tabPane, "style", "frame", ...
    "constraints", createConstraints("border", "center"), ...
    "layout", "gridbag", ...
    "string", style);
    for i = ["top", "bottom", "left", "right"]
        uicontrol(demoFrame, "style", "frame", ...
        "constraints", createConstraints("border", i), ...
        "position", [0, 0, 100, 100]);
    end
    for i = 1:2
        for j = 1:2
            obj = uicontrol(demoFrame, "style", style, ...
            "constraints", createConstraints("gridbag", [i, j, 1, 1], [1,1], "both"), ...
            "string", "I am a "+style+" uicontrol");
            // New Frame capability => insert plot3d
            if style == "frame"
                newaxes(obj);
                plot3d();
            end
        end
    end
end

f.visible = "on";