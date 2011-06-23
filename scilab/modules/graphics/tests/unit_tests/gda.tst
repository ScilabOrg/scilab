// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Pierre LANDO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test gda()

// Check each default property
a = gda();


// VISIBLE
if or(a.visible <> "on") then pause, end


// XAXISVISIBLE, YAXISVISIBLE, ZAXISVISIBLE
if or(a.axes_visible <> ["off" "off" "off"]) then pause, end


// XAXISREVERSE, YAXISREVERSE, ZAXISREVERSE
if or(a.axes_reverse <> ["off" "off" "off"]) then pause, end

// XAXISGRIDCOLOR, YAXISGRIDCOLOR, ZAXISGRIDCOLOR
if or(a.grid <> [-1 -1]) then pause, end



// FONT_STYLE
if or(a.font_style <> 6) then pause, end

// FONT_SIZE
if or(a.font_size <> 1) then pause, end

// FONT_COLOR
if or(a.font_color <> -1) then pause, end

// FONT_FRACTIONAL
if or(a.fractional_font <> "off") then pause, end

// GRIDPOSITION
if or(a.grid_position <> "background") then pause, end


// XAXISLABEL, XAXISLOCATION
// YAXISLABEL, YAXISLOCATION
// ZAXISLABEL, ZAXISLOCATION
// TODO

// XAXISLOGFLAG, YAXISLOGFLAG, ZAXISLOGFLAG
if or(a.log_flags <> "nnn") then pause, end


// XAXISTICKS, YAXISTICKS, ZAXISTICKS
// TODO

// XAXISAUTOTICKS, YAXISAUTOTICKS, ZAXISAUTOTICKS
if or(a.auto_ticks <> ["on","on","on"]) then pause, end


// TITLE
// TODO


// AUTOCLEAR
if or(a.auto_clear <> "off") then pause, end

// FILLED
if or(a.filled <> "on") then pause, end

// BACKGROUND
if or(a.background <> -2) then pause, end

// MARGINS
if or(a.margins <> [0.125, 0.125, 0.125, 0.125]) then pause, end

// AXESBOUNDS
if or(a.axes_bounds <> [0, 0, 1, 1]) then pause, end

// HIDDENCOLOR
if or(a.hiddencolor <> 4) then pause, end





// XAXISNUMBERTICKS, XAXISTICKSLOCATIONS, XAXISTICKSLABELS, XAXISSUBTICKS
// YAXISNUMBERTICKS, YAXISTICKSLOCATIONS, YAXISTICKSLABELS, YAXISSUBTICKS
// ZAXISNUMBERTICKS, ZAXISTICKSLOCATIONS, ZAXISTICKSLABELS, ZAXISSUBTICKS
// AUTOSUBTICKS
// TODO
		
