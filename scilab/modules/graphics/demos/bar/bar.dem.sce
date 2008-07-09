//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file is distributed under the same license as the Scilab package.
//

//===========================================================
// Demo of bar()
// Date : 6/12/2007
//===========================================================

my_handle             = scf(100001);
clf(my_handle,"reset");

// Parameters

x=[1 2 3 4 5 6 7 8 9 10 11 12];
y=[ 13702 16902 22765 ; ..
    13758 19959 21321 ; ..
    16944 23254 24971 ; ..
    16324 18323 0 ; ..
    15399 19648 0 ; ..
    14988 17160 0 ; ..
    10275 10996 0 ; ..
    10195 14316 0 ; ..
    14899 18410 0 ; ..
    18884 23924 0 ; ..
    16919 21535 0; ..
    14759 19979 0];

angle = 295;

BackgroundColorId = color(205,253,203);
year2005ColorId = color(238,222,12);
year2006ColorId = color(255,0,0);
year2007ColorId = color(64,155,59);

// Histogramm drawing

bar(x,y);
drawlater();

// Adjustements
histogramme = gcf();
histogramme.figure_size = [900,600];
histogramme.background = BackgroundColorId;

axes = gca();
axes.background = BackgroundColorId;
axes.x_ticks.labels = ['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';'Aug';'Sep';'Oct';'Nov';'Dec'];
axes.grid=[-1 0];

years = axes.children.children;
year2005 = years(3);
year2006 = years(2);
year2007 = years(1);

year2005.background = year2005ColorId;
year2006.background = year2006ColorId;
year2007.background = year2007ColorId;

year2005.bar_width = 0.25;
year2006.bar_width = 0.25;
year2007.bar_width = 0.25;

// Labels drawing

year2005_data = year2005.data;
year2006_data = year2006.data;
year2007_data = year2007.data;

year2005_shift = year2005.x_shift;
year2006_shift = year2006.x_shift;
year2007_shift = year2007.x_shift;

year2005_width = year2005.bar_width;
year2006_width = year2006.bar_width;
year2007_width = year2007.bar_width;

for j = 1:length(year2005_shift),
  xstring(j+year2005_shift(j)+(year2005_width/3),year2005_data(j,2),string(year2005_data(j,2)),angle);
  t=get("hdl");
  t.font_size=2;
  t.font_style=9;
end;

for j = 1:length(year2006_shift),
  xstring(j+year2006_shift(j)+(year2006_width/3),year2006_data(j,2),string(year2006_data(j,2)),angle);
  t=get("hdl");
  t.font_size=2;
  t.font_style=9;
end;

for j = 1:length(year2007_shift),
  xstring(j+year2007_shift(j)+(year2007_width/3),year2007_data(j,2),string(year2007_data(j,2)),angle);
  t=get("hdl");
  t.font_size=2;
  t.font_style=9;
end;

// Legend drawing

legends(['Year 2005';'Year 2006';'Year 2007'],[year2005ColorId;year2006ColorId;year2007ColorId],[9.5;19500]);
legend_axes = histogramme.children(1);

legend_axes.children.children(2).thickness = 4;
legend_axes.children.children(4).thickness = 4;
legend_axes.children.children(6).thickness = 4;

drawnow();
