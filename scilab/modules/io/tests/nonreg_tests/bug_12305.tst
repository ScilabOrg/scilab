// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
// Copyright (C) 2013 - Test case - Luc GABRIEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 12305 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12305
//
// <-- Short Description -->
//  Legends could not be loaded from SOD file.
//  Test : both graphics must have legends.

currentPath = get_absolute_file_path("bug_12305.tst");

// Calculating the range from different zones
fig=50;LegPos=3;

function [uk]=Plot_xy(f,X,Y,Title,Legend,LegPos)
    //Uitplotten van een X-Y grafiek
    scf(f);drawlater();
    plot2d(X,Y);
    hl=legend(Legend,LegPos);
    c=gcf();c.figure_size=[1600,1000];c.figure_position = [100,100];
    a=gca();a.grid=[1,1];a.font_size=2;
    a.title.text=Title(3);a.title.font_size=2;
    a.x_label.text=Title(1);a.x_label.font_size=2;
    a.y_label.text=Title(2);a.y_label.font_size=2;
    uk=f+1;drawnow();
endfunction
//
//******************************************************************************
//
function[uk]=moving(u,k)
// berekenen van het mov average
//het lopend gemiddelde gebeurt in de kolom richting
//het gemiddelde reduceerd de vector tot n-k+1
//u is de vector met data die uitgemiddeld wordt (kolomwise orientatie)
//k is de stap
//uitfilteren van %nan
    [row,col]=find(isnan(u));u(row,:)=[];
    [row,col]=size(u);
    m=col-k+1;
    uk=zeros(row,m);
    for j=1:k
        uk=uk+u(:,j:m+j-1);
    end;
    uk=uk/k;
endfunction;

//
// Loading data
//
load(currentPath + 'bug_12305_Info.sod');
load(currentPath + 'bug_12305_Measurment.sod');
load(currentPath + 'bug_12305_Statistics.sod');

//
//
//
RangeBack=MaxBack-MinBack;
RangeFront=MaxFront-MinFront;
FiltRangBack=moving(RangeBack',60);FiltRangBack=FiltRangBack';
FiltRangeFront=moving(RangeFront',60);FiltRangeFront=FiltRangeFront';
//
// Calculating the difference between back and front
//
RangeDiff=RangeBack-RangeFront;
FiltRangeDiff=moving(RangeDiff',60);FiltRangeDiff=FiltRangeDiff';
//
// Plotting the eggshell tempeature in front and back
//
scf(fig);subplot(2,1,1);
Title=['X-axis','Y-axis',"Graphic 1"];
Legend=['Data1','Data2','Filtered Data1','Filtered Data2'];
fig=Plot_xy(fig,Timescale(:,1),[AvBack,AvFront],Title,Legend,LegPos);fig=fig-1;
fig=Plot_xy(fig,Timescale(Filt:$,1),[FiltAvBack,FiltAvFront],Title,Legend,LegPos);
a=gca();a.data_bounds=[0,min([AvBack,AvFront])-0.1;Timescale($,1),max([AvBack,AvFront])+0.1];
b=a.children(2).children(2);b.foreground=5;b.thickness=2;
b=a.children(2).children(1);b.foreground=11;b.thickness=2;
b=a.children(3).children(2);b.foreground=29;b.thickness=4;
b=a.children(3).children(1);b.foreground=12;b.thickness=4;
fig=fig-1;subplot(2,1,2);
Title=['X-axis','Y-axis', "Graphic 1"];
Legend=['Data1','Data2'];
fig=Plot_xy(fig,Timescale(:,1),[RangeBack,RangeFront],Title,Legend,LegPos);fig=fig-1;
fig=Plot_xy(fig,Timescale(60:$,1),[FiltRangBack,FiltRangeFront],Title,Legend,LegPos);
a=gca();a.data_bounds=[0,min(MaxBack-MinBack)-0.1;Timescale($,1),max(MaxBack-MinBack)+2];
b=a.children(2).children(2);b.foreground=5;b.thickness=2;
b=a.children(2).children(1);b.foreground=11;b.thickness=2;
b=a.children(3).children(2);b.foreground=29;b.thickness=4;
b=a.children(3).children(1);b.foreground=12;b.thickness=4;
f=gcf();save(TMPDIR + '/bug_12305.sod', 'f');
delete(gcf());
clear();
load(TMPDIR + '/bug_12305.sod');
