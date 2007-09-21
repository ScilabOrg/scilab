//===========================================================
//
// Demo of bar()
// Copyright INRIA
// Author : Pierre MARECHAL
// Scilab team
// Date : 6/12/2005
//
//===========================================================

sdf();
sda();

// Parameters

x=[1 2 3 4 5 6 7 8 9 10 11 12];
y=[ 5555 7687 13588 ; ..
    7654 8003 13597 ; ..
    8231 10450 16772 ; ..
    791 9124 16169 ; ..
    7111 11099 15267 ; ..
    6812 10718 14855 ; ..
    3919 13869 10147 ; ..
    3809 10628 9998 ; ..
    7450 13022 14445 ; ..
    9039 16442 18212 ; ..
    7708 15116 16777; ..
    6223 11384 13897];

angle = 295;

BackgroundColorId = color(205,253,203);
year2003ColorId = color(238,222,12);
year2004ColorId = color(255,0,0);
year2005ColorId = color(64,155,59);

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
year2003 = years(3);
year2004 = years(2);
year2005 = years(1);

year2003.background = year2003ColorId;
year2004.background = year2004ColorId;
year2005.background = year2005ColorId;

year2003.bar_width = 0.25;
year2004.bar_width = 0.25;
year2005.bar_width = 0.25;

// Labels drawing

year2003_data = year2003.data;
year2004_data = year2004.data;
year2005_data = year2005.data;

year2003_shift = year2003.x_shift;
year2004_shift = year2004.x_shift;
year2005_shift = year2005.x_shift;

year2003_width = year2003.bar_width;
year2004_width = year2004.bar_width;
year2005_width = year2005.bar_width;

for j = 1:length(year2003_shift),
  xstring(j+year2003_shift(j)+(year2003_width/3),year2003_data(j,2),string(year2003_data(j,2)),angle);
  t=get("hdl");
  t.font_size=2;
  t.font_style=9;
end;

for j = 1:length(year2004_shift),
  xstring(j+year2004_shift(j)+(year2004_width/3),year2004_data(j,2),string(year2004_data(j,2)),angle);
  t=get("hdl");
  t.font_size=2;
  t.font_style=9;
end;

for j = 1:length(year2005_shift),
  xstring(j+year2005_shift(j)+(year2005_width/3),year2005_data(j,2),string(year2005_data(j,2)),angle);
  t=get("hdl");
  t.font_size=2;
  t.font_style=9;
end;

// Legend drawing

legends(['Year 2003';'Year 2004';'Year 2005'],[year2003ColorId;year2004ColorId;year2005ColorId],[9.5;19500]);
legend_axes = histogramme.children(1);

legend_axes.children.children(2).thickness = 4;
legend_axes.children.children(4).thickness = 4;
legend_axes.children.children(6).thickness = 4;

// Toolbar removed

toolbar(0,'off');
a=gca(); a.title.text='bar histogram'; a.title.font_size=3;,
drawnow();

