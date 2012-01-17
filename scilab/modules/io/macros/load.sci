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
    if typeof(varValue(i)) == "handle" then
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

for i = 1:prod(matrixHandle.dims)
    h($+1) = createSingleHandle(matrixHandle.values(i));
    if isList(matrixHandle.values(i).user_data) then
        h($).user_data = inspectList(matrixHandle.values(i).user_data)
    elseif typeof(matrixHandle.values(i).user_data) == "handle" then
        h($).user_data = createMatrixHandle(matrixHandle.values(i).user_data)
    end
end
h = matrix(h, matrixHandle.dims);
endfunction

function h = createSingleHandle(item)
disp(item.type);
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
for i = 1:size(fields, "*")
    if fields(i) == "children" then
        createMatrixHandle(figureProperties(fields(i)));
    elseif fields(i) == "figure_id" then
        // Set at creation
    else
        h(fields(i)) = figureProperties(fields(i));
    end
end
endfunction

//
// LABEL
//
function h = createLabel(labelProperties, h)
// TODO
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
endfunction

//
// POLYLINE
//
function h = createPolyline(polylineProperties)
// TODO
fields = fieldnames(polylineProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = polylineProperties(fields(i));
end
endfunction

//
// PLOT3D
//
function h = createPlot3d(plot3dProperties)
// plot3d modify the given rotation angles
// trick to force keeping the good rotation angles F.Leray 18.02.05
// same issue with axes properties... B.Jofret 21.04.09
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

if or(plot3dProperties.color_flag==[2 5]) then
    plot3d1(plot3dProperties.data.x, plot3dProperties.data.y, list(plot3dProperties.data.z, plot3dProperties.data.color))
else
    plot3d(plot3dProperties.data.x,plot3dProperties.data.y,plot3dProperties.data.z)
end

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
set(h,"visible",plot3dProperties.visible)
set(h,"surface_mode",plot3dProperties.surface_mode)
set(h,"thickness",plot3dProperties.thickness)
set(h,"foreground",plot3dProperties.foreground),
set(h,"color_mode",plot3dProperties.color_mode),
set(h,"mark_style",plot3dProperties.mark_style),
set(h,"mark_size",plot3dProperties.mark_size),
set(h,"mark_size_unit",plot3dProperties.mark_size_unit),
set(h,"mark_foreground",plot3dProperties.mark_foreground),
set(h,"mark_background",plot3dProperties.mark_background)
set(h,"mark_mode",plot3dProperties.mark_mode)
set(h,"color_flag",plot3dProperties.color_flag),
set(h,"hiddencolor",plot3dProperties.hiddencolor),

if plot3dProperties.clip_state == "on" then
    set(h,"clip_box", plot3dProperties.clip_box);
end
set(h,"clip_state",plot3dProperties.clip_state);
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
// TODO
fields = fieldnames(surfaceProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = surfaceProperties(fields(i));
end
endfunction

//
// COMPOUND
//
function h = createCompound(compoundProperties)
// TODO
fields = fieldnames(compoundProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    if fields(i) == "children" then
        h(fields(i)) = createMatrixHandle(compoundProperties(fields(i)));
    else
        h(fields(i)) = compoundProperties(fields(i));
    end
end
endfunction

//
// RECTANGLE
//
function h = createRectangle(rectangleProperties)
// TODO
fields = fieldnames(rectangleProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = rectangleProperties(fields(i));
end
endfunction

//
// ARC
//
function h = createArc(arcProperties)
// TODO
fields = fieldnames(arcProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = arcProperties(fields(i));
end
endfunction

//
// CHAMP
//
function h = createChamp(champProperties)
// TODO
fields = fieldnames(champProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = champProperties(fields(i));
end
endfunction

//
// SEG
//
function h = createSeg(segsProperties)
// TODO
fields = fieldnames(segsProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = segsProperties(fields(i));
end
endfunction

//
// GRAYPLOT
//
function h = createGrayplot(grayplotProperties)
// TODO
fields = fieldnames(grayplotProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = grayplotProperties(fields(i));
end
endfunction

//
// MATPLOT
//
function h = createMatplot(matplotProperties)
// TODO
fields = fieldnames(matplotProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = matplotProperties(fields(i));
end
endfunction

//
// FEC
//
function h = createFec(fecProperties)
// TODO
fields = fieldnames(fecProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = fecProperties(fields(i));
end
endfunction

//
// LEGEND
//
function h = createLegend(legendProperties)
// TODO
fields = fieldnames(legendProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    if fields(i) == "links" then
        h(fields(i)) = createMatrixHandle(legendProperties(fields(i)));
    else
        h(fields(i)) = legendProperties(fields(i));
    end
end
endfunction

//
// TEXT
//
function h = createText(textProperties)
// TODO
fields = fieldnames(textProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = textProperties(fields(i));
end
endfunction

//
// AXIS
//
function h = createAxis(axisProperties)
// TODO
fields = fieldnames(axisProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = axisProperties(fields(i));
end
endfunction

//
// uimenu
//
function h = createuimenu(uimenuProperties)
// TODO
fields = fieldnames(uimenuProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    if fields(i) == "children" then
        h(fields(i)) = createMatrixHandle(uimenuProperties(fields(i)));
    else
        h(fields(i)) = uimenuProperties(fields(i));
    end
end
endfunction

//
// UICONTEXTMENU
//
function h = createuicontextmenu(uicontextmenuProperties)
// TODO
fields = fieldnames(uicontextmenuProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    if fields(i) == "children" then
        h(fields(i)) = createMatrixHandle(uicontextmenuProperties(fields(i)));
    else
        h(fields(i)) = uicontextmenuProperties(fields(i));
    end
end
endfunction

//
// uicontrol
//
function h = createuicontrol(uicontrolProperties)
// TODO
fields = fieldnames(uicontrolProperties);
fields(1) = [];
for i = 1:size(fields, "*")
    h(fields(i)) = uicontrolProperties(fields(i));
end
endfunction
