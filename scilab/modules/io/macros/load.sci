// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function load(filename, varargin)

// Store input arguments as global variables
global %__varargin__ %__filename__
%__varargin__ = varargin;
%__filename__ = filename;

// Clear environment to be sure to load variables even if they already exist
clear

// Restore input arguments from global variables
global %__varargin__ %__filename__
%__varargin__ = %__varargin__;
%__filename__ = %__filename__;

%__nbNamesBefore__ = size(who("get"), "*");

%__isHdf5Format__ = %T;
%__ierr__ = execstr("import_from_hdf5(pathconvert(%__filename__, %F, %T))", "errcatch");

if %__ierr__<>0 then
    %__isHdf5Format__ = %F;
    %_load(%__filename__, %__varargin__(:));
end
clear %__ierr__ ans // ans is the output status of import_from_hdf5
%__namesAfter__ = who("get");

%__createdNames__ = %__namesAfter__(1:(size(%__namesAfter__, "*") - %__nbNamesBefore__ - 2)); // -2 for %__nbNamesBefore__ & %__isHdf5Format__

%__createdValues__ = list();
for %__variableIndex = 1:size(%__createdNames__, "*")
    %__createdValues__($+1) = evstr(%__createdNames__(%__variableIndex));
end

if %__isHdf5Format__ then
    [%__createdNames__, %__createdValues__] = %__selectVariables__(%__createdNames__, %__createdValues__, %__varargin__);
end

execstr("[" + strcat(%__createdNames__, ",") + "] = resume(%__createdValues__(:))");

// Clear temporary variables input arguments from global variables
clearglobal %__varargin__ %__filename__

endfunction

function [varNames, varValues] = %__selectVariables__(varNames, varValues, loadVarargin)

// Get the names the user wants to load
userNames = [];
for kArg = 1:size(loadVarargin)
    userNames($+1) = loadVarargin(kArg);
end

// Remove the variables the user does not want
if ~isempty(userNames) then
    kVar = 1;
    while kVar <= size(varNames, "*")
        if ~or(varNames(kVar)==userNames) then // Not wanted
            varNames(kVar) = [];
            varValues(kVar) = null();
        else
            kVar = kVar + 1;
        end
    end
end

// Manage handles properly
varValues = %__convertHandles__(varValues);
endfunction

function [varValues] = %__convertHandles__(varValues)
for i = 1:size(varValues)
    if typeof(varValues(i)) == "ScilabMatrixHandle" then
        //convert handle to tlist
        varValues(i) = createMatrixHandle(varValues(i));
    elseif isList(varValues(i)) then
        //list container
        varValues(i) = inspectList(varValues(i));
    end
end
endfunction

function result = isList(var)
    //15 : list
    //16 : tlist
    //17 : mlist
    if or(type(var) == [15, 16, 17]) then
        result  = %t;
    else
        result = %f;
    end
endfunction

function varValue = inspectList(varValue)
for i = 1:size(varValue)
    if typeof(varValue(i)) == "ScilabMatrixHandle" then
        varValue(i) = createMatrixHandle(varValue(i));
    elseif isList(varValue(i)) then
        varValue(i) = inspectList(varValue(i));
    else
        varValue(i) = varValue(i);
    end
end
endfunction

function h = createMatrixHandle(matrixHandle)
h = [];
if typeof(matrixHandle) <> "ScilabMatrixHandle" then
    return;
end

for i = prod(matrixHandle.dims):-1:1
    h($+1) = createSingleHandle(matrixHandle.values(i));
    if or(fieldnames(matrixHandle.values(i))=="user_data") then // TODO Remove after graphic branch merge
        if isList(matrixHandle.values(i).user_data) then
            h($).user_data = inspectList(matrixHandle.values(i).user_data)
        elseif typeof(matrixHandle.values(i).user_data) == "ScilabMatrixHandle" then
            h($).user_data = createMatrixHandle(matrixHandle.values(i).user_data)
        end
    end
end
h = matrix(h, matrixHandle.dims);
endfunction

function h = createSingleHandle(item)
    select item.type
    case "Figure"
        h = createFigure(item);
    case "Axes"
        h = createAxes(item);
    case "Polyline"
        h = createPolyline(item);
    case "Plot3d"
        h = createPlot3d(item);
    case "Fac3d"
        h = createFac3d(item);
    case "Compound"
        h = createCompound(item);
    case "Rectangle"
        h = createRectangle(item);
    case "Arc"
        h = createArc(item);
    case "Champ"
        h = createChamp(item);
    case "Segs"
        h = createSegs(item);
    case "Grayplot"
        h = createGrayplot(item);
    case "Matplot"
        h = createMatplot(item);
    case "Fec"
        h = createFec(item);
    case "Legend"
        h = createLegend(item);
    case "Text"
        h = createText(item);
    case "Axis"
        h = createAxis(item);
    case "uimenu"
        h = createuimenu(item);
    case "uicontextmenu"
        h = createuicontextmenu(item);
    case "uicontrol"
        h = createuicontrol(item);
    else
        error("handle of type " + item.type + " unhandled");
        h = [];
    end
endfunction

//
// FIGURE
//
function h = createFigure(figureProperties)
fields = fieldnames(figureProperties);
fields(1) = [];

h = scf(figureProperties.figure_id);
fields(fields=="figure_id") = [];

h.figure_position=figureProperties.figure_position;
fields(fields=="figure_position") = [];
// set auto_resize first otherwise viewport modification may not have any effect.
h.auto_resize = figureProperties.auto_resize;
fields(fields=="auto_resize") = [];
h.figure_size = figureProperties.figure_size;
fields(fields=="figure_size") = [];
// set axes_size last because it's more important than figure_size
h.axes_size = figureProperties.axes_size;
fields(fields=="axes_size") = [];

for i = 1:size(fields, "*")
    if fields(i) == "children" then
        createMatrixHandle(figureProperties(fields(i)));
    else
        h(fields(i)) = figureProperties(fields(i));
    end
end
endfunction

//
// LABEL
//
function h = createLabel(labelProperties, h)
fields = fieldnames(labelProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = labelProperties(fields(i));
end
endfunction

//
// TICKS
//
function h = createTicks(ticksProperties)
h = tlist(['ticks','locations','labels'], [], []);
fields = fieldnames(ticksProperties);
for i = 1:size(fields, "*")
    h(fields(i)) = ticksProperties(fields(i));
end
endfunction

//
// AXES
//
function h = createAxes(axesProperties)
h = gca();
fields = fieldnames(axesProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    if or(fields(i) == ["title","x_label","y_label","z_label"]) then
        createLabel(axesProperties(fields(i)), h(fields(i)));
    elseif or(fields(i) == ["x_ticks", "y_ticks", "z_ticks"]) then
        h(fields(i)) = createTicks(axesProperties(fields(i)));
    elseif fields(i) == "children" then
        createMatrixHandle(axesProperties(fields(i)));
    elseif fields(i) == "clip_state" then
        if axesProperties.clip_state=="on" then
            set(h,"clip_box",axesProperties.clip_box);
        end
        set(h,"clip_state", axesProperties.clip_state);
    elseif fields(i) == "clip_box" then
        // managed with 'clip_state'
    else
        h(fields(i)) = axesProperties(fields(i));
    end
end

// Legend management
global %LEG
if ~isempty(%LEG) then
    // Get handles from paths
    links=get_links_from_path(h, %LEG.paths)
    if ~isempty(links) then
        L = captions(links, %LEG.text)
        L.visible         = %LEG.visible
        L.font_style      = %LEG.font_style
        L.font_size       = %LEG.font_size
        L.font_color      = %LEG.font_color
        L.fractional_font = %LEG.fractional_font
        L.mark_mode       = 'off';
        L.legend_location = %LEG.legend_location
        L.position        = %LEG.position
        L.line_mode       = %LEG.line_mode
        L.thickness       = %LEG.thickness
        L.foreground      = %LEG.foreground
        L.fill_mode       = %LEG.fill_mode
        L.background      = %LEG.background
        if %LEG.clip_state=="on" then
            L.clip_box      = %LEG.clip_box
        end
        L.clip_state      = %LEG.clip_state
        L.user_data       = %LEG.user_data
    else
        warning(msprintf(_("%s: Legend does not fit with the current context. Skipped\n"), "load"));
    end
end
clearglobal %LEG
endfunction

//
// POLYLINE
//
function h = createPolyline(polylineProperties)
fields = fieldnames(polylineProperties);
fields(1) = [];

xpoly(polylineProperties.data(:,1), polylineProperties.data(:,2))

h = gce();

if polylineProperties.clip_state=="on" then
    set(h, "clip_box", polylineProperties.clip_box)
end
set(h, "clip_state", polylineProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

if polylineProperties.interp_color_mode=="on" & ~isempty(polylineProperties.interp_color_vector) then
    set(h, "interp_color_vector", polylineProperties.interp_color_vector);
    set(h, "interp_color_mode", polylineProperties.interp_color_mode);
else
    if ~isempty(polylineProperties.interp_color_vector) then
        h.interp_color_vector = polylineProperties.interp_color_vector;
    end
    h.interp_color_mode = polylineProperties.interp_color_mode;
end
fields(fields=="interp_color_vector") = [];
fields(fields=="interp_color_mode") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = polylineProperties(fields(i));
end
endfunction

//
// PLOT3D
//
function h = createPlot3d(plot3dProperties)
h = createSurface(plot3dProperties);
endfunction

//
// FAC3D
//
function h = createFac3d(fac3dProperties)
h = createSurface(fac3dProperties);
endfunction

//
// SURFACE
//
function h = createSurface(surfaceProperties)
// plot3d modify the axes properties
//  - Save it
// - Draw plot3d
// - Restore it
a=gca();
rotation_angles = a.rotation_angles;
axes_visible = a.axes_visible;
box = a.box;
margins = a.margins;
x_label_visible = a.x_label.visible;
y_label_visible = a.y_label.visible;
z_label_visible = a.z_label.visible;
x_label_text = a.x_label.text;
y_label_text = a.y_label.text;
z_label_text = a.z_label.text;
axes_isoview = a.isoview;

if (or(surfaceProperties.color_flag==[2 5]) & ~or(fields=="cdata_mapping")) | ..
    ((surfaceProperties.color_flag>=2) & or(fields=="cdata_mapping")) then
    plot3d1(surfaceProperties.data.x, surfaceProperties.data.y, list(surfaceProperties.data.z, surfaceProperties.data.color))
else
    plot3d(surfaceProperties.data.x,surfaceProperties.data.y,surfaceProperties.data.z)
end
fields(fields=="data") = [];

// Restore this properties after plot3d.
a.rotation_angles = rotation_angles;
a.axes_visible = axes_visible;
a.box = box;
a.margins = margins;
a.x_label.visible = x_label_visible;
a.y_label.visible = y_label_visible;
a.z_label.visible = z_label_visible;
a.x_label.text = x_label_text;
a.y_label.text = y_label_text;
a.z_label.text = z_label_text;
a.isoview = axes_isoview;

h=gce();

if or(fields=="cdata_mapping") then // Fac3d specific
    if surfaceProperties.color_flag >= 2 then
        set(h, "cdata_mapping", surfaceProperties.cdata_mapping);
    end
    fields(fields=="cdata_mapping") = [];
end

if surfaceProperties.clip_state == "on" then
    set(h,"clip_box", surfaceProperties.clip_box);
end
set(h,"clip_state",surfaceProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = surfaceProperties(fields(i));
end
endfunction

//
// COMPOUND
//
function h = createCompound(compoundProperties)
fields = fieldnames(compoundProperties);
fields(1) = [];

h = glue(createMatrixHandle(compoundProperties.children));
fields(fields=="children") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = compoundProperties(fields(i));
end
endfunction

//
// RECTANGLE
//
function h = createRectangle(rectangleProperties)
fields = fieldnames(rectangleProperties);
fields(1) = [];

xrect(0,1,1,1); // create the rectangle with dummy values
h = gce();

if rectangleProperties.clip_state == "on" then
    set(h,"clip_box", rectangleProperties.clip_box);
end
set(h,"clip_state",rectangleProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = rectangleProperties(fields(i));
end
endfunction

//
// ARC
//
function h = createArc(arcProperties)
fields = fieldnames(arcProperties);
fields(1) = [];

xarc(0,1,1,1,0,360); // create the arc with dummy values
h = gce();

if arcProperties.clip_state == "on" then
    set(h,"clip_box", arcProperties.clip_box);
end
set(h,"clip_state",arcProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = arcProperties(fields(i));
end
endfunction

//
// CHAMP
//
function h = createChamp(champProperties)
fields = fieldnames(champProperties);
fields(1) = [];

champ(champProperties.data.x, champProperties.data.y, champProperties.data.fx, champProperties.data.fy);
fields(fields=="data") = [];

h=gce();

if champProperties.clip_state == "on" then
    set(h,"clip_box", champProperties.clip_box);
end
set(h,"clip_state",champProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = champProperties(fields(i));
end
endfunction

//
// SEG
//
function h = createSegs(segsProperties)
fields = fieldnames(segsProperties);
fields(1) = [];

xsegs(segsProperties.data(:,1), segsProperties.data(:,2))

h=gce()

if segsProperties.clip_state == "on" then
    set(h,"clip_box", segsProperties.clip_box);
end
set(h,"clip_state",segsProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = segsProperties(fields(i));
end
endfunction

//
// GRAYPLOT
//
function h = createGrayplot(grayplotProperties)
fields = fieldnames(grayplotProperties);
fields(1) = [];

grayplot(grayplotProperties.data.x, grayplotProperties.data.y, grayplotProperties.data.z);
fields(fields=="data") = [];

h = gce();

if grayplotProperties.clip_state=="on" then
    set(h, "clip_box", grayplotProperties.clip_box)
end
set(h, "clip_state", grayplotProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = grayplotProperties(fields(i));
end
endfunction

//
// MATPLOT
//
function h = createMatplot(matplotProperties)
fields = fieldnames(matplotProperties);
fields(1) = [];

Matplot(matplotProperties.data);
fields(fields=="data") = [];

h = gce();

if matplotProperties.clip_state=="on" then
    set(h, "clip_box", matplotProperties.clip_box)
end
set(h, "clip_state", matplotProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = matplotProperties(fields(i));
end
endfunction

//
// FEC
//
function h = createFec(fecProperties)
fields = fieldnames(fecProperties);
fields(1) = [];

fec(fecProperties.data(:,1), fecProperties.data(:,2), fecProperties.triangles, fecProperties.data(:,3));
fields(fields=="data") = [];
fields(fields=="triangles") = [];

h = unglue(gce());

if fecProperties.clip_state=="on" then
    set(h, "clip_box", fecProperties.clip_box)
end
set(h, "clip_state", fecProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = fecProperties(fields(i));
end
endfunction

//
// LEGEND
//
function h = createLegend(legendProperties)
global %LEG
%LEG = legendProperties;
endfunction

//
// TEXT
//
function h = createText(textProperties)
fields = fieldnames(textProperties);
fields(1) = [];

if textProperties.text_box_mode == 'off' then
    xstring(textProperties.data(1), textProperties.data(2), textProperties.text)
else
    xstringb(textProperties.data(1), textProperties.data(2), textProperties.text, textProperties.text_box(1), textProperties.text_box(2))
end

h = gce();

if textProperties.clip_state=="on" then
    set(h, "clip_box", textProperties.clip_box)
end
set(h, "clip_state", textProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = textProperties(fields(i));
end
endfunction

//
// AXIS
//
function h = createAxis(axisProperties)
fields = fieldnames(axisProperties);
fields(1) = [];

if axisProperties.tics_direction == "bottom" then
    axisdir='d';
elseif tics_direction == "top" then
    axisdir='u';
elseif tics_direction == "left" then
    axisdir='l';
elseif tics_direction == "right" then
    axisdir='r';
elseif nx>1 then
    axisdir='u';
else
    axisdir='l';
end
fields(fields=="tics_direction") = [];

drawaxis(x=axisProperties.xtics_coord,y=axisProperties.ytics_coord,dir=axisdir);
fields(fields=="xtics_coord") = [];
fields(fields=="ytics_coord") = [];

h=gce()

if axisProperties.clip_state=="on" then
    set(h, "clip_box", axisProperties.clip_box)
end
set(h, "clip_state", axisProperties.clip_state);
fields(fields=="clip_box") = [];
fields(fields=="clip_state") = [];

for i = 1:size(fields, "*")
    h(fields(i)) = axisProperties(fields(i));
end
endfunction

//
// uimenu
//
function h = createuimenu(uimenuProperties)
fields = fieldnames(uimenuProperties);
fields(1) = [];

h = uimenu();

for i = 1:size(fields, "*")
    if fields(i) == "children" then
        children = createMatrixHandle(uimenuProperties(fields(i)));
        for k=1:size(children, "*")
            set(children(k), "parent", h);
        end
    else
        h(fields(i)) = uimenuProperties(fields(i));
    end
end
endfunction

//
// UICONTEXTMENU
//
function h = createuicontextmenu(uicontextmenuProperties)
fields = fieldnames(uicontextmenuProperties);
fields(1) = [];

h = uicontextmenu();

for i = 1:size(fields, "*")
    if fields(i) == "children" then
        children = createMatrixHandle(uicontextmenuProperties(fields(i)));
        for k=1:size(children, "*")
            set(children(k), "parent", h);
        end
    else
        h(fields(i)) = uicontextmenuProperties(fields(i));
    end
end
endfunction

//
// uicontrol
//
function h = createuicontrol(uicontrolProperties)

fields = fieldnames(uicontrolProperties);
fields(1) = [];

h = uicontrol("Style", uicontrolProperties.style);
fields(fields=="style") = [];

for i = 1:size(fields, "*")
    if fields(i) == "children" then
        children = createMatrixHandle(uicontrolProperties(fields(i)));
        for k=1:size(children, "*")
            set(children(k), "parent", h);
        end
    else
        h(fields(i)) = uicontrolProperties(fields(i));
    end
end
endfunction

// Utility function for legends, copy/paste from %h_load
function links=get_links_from_path(ax,paths)
//  ax is a  handle on an axes entity
//  paths a list or row vector which gives the set of paths relative to
//  the axes
links=[];
ok=%t;
for p=paths
    e=ax;
    p(1)=p(1)-1// the caption does not exists yet
    for kp=1:size(p,'*'),
        if or(e.type==['Axes','Compound'])&p(kp)<=size(e.children,'*') then
            e=e.children(p(kp)),
        else
            ok=%f
            break
        end
    end
    if ~ok then
        break
    end
    links=[links,e]
end
if ~ok then
    links=[];
end
endfunction
