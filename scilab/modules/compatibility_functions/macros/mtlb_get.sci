// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function v=mtlb_get(H,property)

  // Check input parameters
  if argn(2) <> 2 then
      error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"mtlb_get",2))
  end

  v         = [];
  oldfigure = gcf();
  scf(H);
  a         = gca();

  if or(convstr(property) == ["currentaxes";           ..
                              "currentcharacter";      ..
                              "currentmenu";           ..
                              "currentobject";         ..
                              "currentpoint";          ..
                              "fixedcolors";           ..
                              "inverthardcopy";        ..
                              "keypressfcn";           ..
                              "menubar";               ..
                              "name";                  ..
                              "nextplot";              ..
                              "numbertitle";           ..
                              "paperunits";            ..
                              "paperorientation";      ..
                              "paperposition";         ..
                              "papersize";             ..
                              "papertype";             ..
                              "resizefcn";             ..
                              "selectiontype";         ..
                              "sharecolors";           ..
                              "units";                 ..
                              "windowbuttondownfcn";   ..
                              "windowbuttonmotionfcn"; ..
                              "windowbuttonupfcn";     ..
                              "buttondownfcn";         ..
                              "children";              ..
                              "interruptible";         ..
                              "parent";                ..
                              "userdata"                ] ) then
    error(msprintf(gettext("%s: No equivalent for ""%s"" property.\n"), "mtlb_get", property));
  end

  select convstr(property)
    case "backingstore" then
    case "color" then
    case "colormap" then
      v=f.color_map;
    case "mincolormap" then
      v = f.color_map;
      v = size(v,1);
    case "pointer" then
      v="arrow";
    case "position" then
      o  = f.figure_position;
      sz = f.figure_size;
      v  = [o(:);sz(:)]'
    case "resize" then
      v="on";
    case "clipping" then
      v=a.clip_box;
      if v<>[] & v(1)<>0 then
        v="on";
      else
        v = "off";
      end
    case "type" then
      v="figure";
    case "visible" then
      v="on";
  end

  scf(oldfigure);

endfunction
