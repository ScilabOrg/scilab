//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=CCS(job,arg1,arg2)
    // the automatically generated interface block for Modelica CCS.mo model
    //   - avec un dialogue de saisie de parametre
    x=[];y=[];typ=[];
    select job
    case "plot" then
        standard_draw(arg1,%f,CCS_draw_ports)
    case "getinputs" then
        [x,y,typ]=CCS_inputs(arg1)
    case "getoutputs" then
        [x,y,typ]=CCS_outputs(arg1)
    case "getorigin" then
        [x,y]=standard_origin(arg1)
    case "set" then
        x=arg1;
        graphics=arg1.graphics;exprs=graphics.exprs
        model=arg1.model;
        x=arg1
    case "define" then
        ModelName="CCS"
        PrametersValue=[]
        ParametersName=[]
        model=scicos_model()
        Typein=[];Typeout=[];MI=[];MO=[]
        P=[2,50,1,0; 70,98,2,0;70,2,-2,0]

        PortName=["Iin";"p";"n"]

        for i=1:size(P,"r")
            if P(i,3)==1  then  Typein= [Typein; "E"];MI=[MI;PortName(i)];end
            if P(i,3)==2  then  Typein= [Typein; "I"];MI=[MI;PortName(i)];end
            if P(i,3)==-1 then  Typeout=[Typeout;"E"];MO=[MO;PortName(i)];end
            if P(i,3)==-2 then  Typeout=[Typeout;"I"];MO=[MO;PortName(i)];end
        end
        model=scicos_model()
        mo=modelica()
        model.sim=ModelName;
        mo.inputs=MI;
        mo.outputs=MO;
        model.rpar=PrametersValue;
        mo.parameters=list(ParametersName,PrametersValue,zeros(ParametersName));
        exprs=[]
        gr_i=["";"if orient then";"  xpolys(orig(1)+[0.7142857,0.7142857,0.3714286,0.0571429,0.3714286;0.7142857,0.7142857,0.4285714,0.4142857,0.4285714]*sz(1),orig(2)+[0.7,0,0.53,0.49,0.45;0.98,0.3,0.49,0.49,0.49]*sz(2),[2,2,6,6,6])";"  xstring(orig(1)+0.7714286*sz(1),orig(2)+0.75*sz(2),""CCS"")";"  xpolys(orig(1)+[0.7142857,0.7142857,0.7142857;0.7142857,0.6,0.8285714]*sz(1),orig(2)+[0.35,0.63,0.63;0.63,0.55,0.55]*sz(2),[5,5,5])";"  xrects([orig(1)+0.4285714*sz(1); orig(2)+0.7*sz(2);0.5714286*sz(1);0.4*sz(2)],0)";"else";"  xpolys(orig(1)+[0.2857143,0.2857143,0.6285714,0.9428571,0.6285714;0.2857143,0.2857143,0.5714286,0.5857143,0.5714286]*sz(1),orig(2)+[0.7,0,0.53,0.49,0.45;0.98,0.3,0.49,0.49,0.49]*sz(2),[2,2,6,6,6])";"  xstring(orig(1)+sz(1)-(0.7714286*sz(1)),orig(2)+0.75*sz(2),""CCS"")";"  xpolys(orig(1)+[0.2857143,0.2857143,0.2857143;0.2857143,0.4,0.1714286]*sz(1),orig(2)+[0.35,0.63,0.63;0.63,0.55,0.55]*sz(2),[5,5,5])";"  xrects([orig(1)+0*sz(1); orig(2)+0.7*sz(2);0.5714286*sz(1);0.4*sz(2)],0)";"end"]
        model.blocktype="c"
        model.dep_ut=[%f %t]
        mo.model=ModelName
        model.equations=mo
        model.in=ones(size(MI,"*"),1)
        model.out=ones(size(MO,"*"),1)
        x=standard_define([2.1,3],model,exprs,list(gr_i,0))
        x.graphics.in_implicit=Typein;
        x.graphics.out_implicit=Typeout;
    end
endfunction
//=========================
function CCS_draw_ports(o)
    [orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
    xset("pattern",default_color(0))
    // draw input/output ports
    //------------------------
    // [x_in_Icon,y_in_Icon,type(2=imp_in/-2:imp_out/1=exp_input/-1_exp_output),orientation(degree)]
    P=[2,50,1,0; 70,98,2,0;70,2,-2,0]

    //============================
    // setting the input/ outputs and direction
    // implicit port: if it's located in the right it's output and while,
    // else black
    // explicit ports:

    in=  [-1 -1; 1  0;-1  1; -1 -1; -1 0]*diag([xf/28,yf/28]) ;// left_triangle
    out= [-1 -1; 1  0;-1  1; -1 -1;  1 0]*diag([xf/28,yf/28]) ;// downward_triangle
    in2= [-1 -1; 1 -1; 1  1; -1  1; -1 -1; 0 0]*diag([xf/28,yf/28])
    out2=[ 1  1;-1  1;-1 -1;  1 -1;  1  1; 0 0]*diag([xf/28,yf/28])

    xset("pattern",default_color(1))
    xset("thickness",1)

    if orient then
        for i=1:size(P,"r")
            theta=P(i,4)*%pi/180;
            R=[cos(theta),sin(theta);sin(-theta),cos(theta)];

            if P(i,3)==1 then // explicit
                inR=in*R;
                xfpoly(orig(1)+inR(:,1)+P(i,1)*sz(1)/100,orig(2)+inR(:,2)+P(i,2)*sz(2)/100,1)
            end

            if  P(i,3)==-1 then
                outR=out*R;
                xfpoly(orig(1)+outR(:,1)+P(i,1)*sz(1)/100,orig(2)+outR(:,2)+P(i,2)*sz(2)/100,1)
            end

            if P(i,3)==2 then  // deciding the port's color: black, if x<sz(1)/2 else white.
                in2R=in2*R;
                xfpoly(orig(1)+in2R(:,1)+P(i,1)*sz(1)/100,orig(2)+  in2R(:,2)+P(i,2)*sz(2)/100,1)
            end

            if P(i,3)==-2 then  // deciding the port's color: black, if x<sz(1)/2 else white.
                out2R=out2*R;
                xpoly(orig(1)+out2R(:,1)+P(i,1)*sz(1)/100,orig(2)+  out2R(:,2)+P(i,2)*sz(2)/100, "lines",1)
            end
        end
    else
        for i=1:size(P,"r")
            theta=P(i,4)*%pi/180;
            R=[cos(theta),sin(theta);sin(-theta),cos(theta)];

            if P(i,3)==1 then // explicit
                inR=in*R;
                xfpoly(orig(1)+sz(1)-inR(:,1)-P(i,1)*sz(1)/100,orig(2)+inR(:,2)+P(i,2)*sz(2)/100,1)
            end
            if P(i,3)==-1 then // explicit
                outR=out*R;
                xfpoly(orig(1)+sz(1)-outR(:,1)-P(i,1)*sz(1)/100,orig(2)+outR(:,2)+P(i,2)*sz(2)/100,1)
            end

            if P(i,3)==2 then  // deciding the port's color: black, if x<sz(1)/2 else white.
                in2R=in2*R;
                xfpoly(orig(1)+sz(1)-in2R(:,1)-P(i,1)*sz(1)/100,orig(2)+  in2R(:,2)+P(i,2)*sz(2)/100,1)
            end
            if P(i,3)==-2 then  // deciding the port's color: black, if x<sz(1)/2 else white.
                out2R=out2*R;
                xpoly(orig(1)+sz(1)-out2R(:,1)-P(i,1)*sz(1)/100,orig(2)+  out2R(:,2)+P(i,2)*sz(2)/100, "lines",1)
            end
        end
    end
endfunction
//=========================
function [x,y,typ]=CCS_inputs(o)
    // Copyright INRIA
    xf=60
    yf=40
    [orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
    //[orig,sz,orient]=o(2)(1:3);
    inp=size(o.model.in,1);clkinp=size(o.model.evtin,1);

    // [x_in_Icon,y_in_Icon,type(2=imp/1=exp_input/-1_exp_output),orientation(degree)]
    P=[2,50,1,0; 70,98,2,0;70,2,-2,0]
    in=  [-1 -1; 1  0;-1  1; -1 -1; -1 0]*diag([xf/28,yf/28]) ;// left_triangle
    out= [-1 -1; 1  0;-1  1; -1 -1;  1 0]*diag([xf/28,yf/28]) ;// downward_triangle
    in2= [-1 -1; 1 -1; 1  1; -1  1; -1 -1; 0 0]*diag([xf/28,yf/28])
    out2=[ 1  1;-1  1;-1 -1;  1 -1;  1  1; 0 0]*diag([xf/28,yf/28])

    x=[];y=[];typ=[]
    if orient then
        for i=1:size(P,"r")
            theta=P(i,4)*%pi/180;
            R=[cos(theta),sin(theta);sin(-theta),cos(theta)];
            if (P(i,3))==1 then // explicit_input
                inR=in($,:)*R;
                x=[x,orig(1)+inR(:,1)+P(i,1)*sz(1)/100];
                y=[y,orig(2)+inR(:,2)+P(i,2)*sz(2)/100];
                typ=[typ,1];
            end
            if(P(i,3)==2) then  // implicit
                in2R=in2($,:)*R;
                x=[x,orig(1)+in2R(:,1)+P(i,1)*sz(1)/100];// Black
                y=[y,orig(2)+in2R(:,2)+P(i,2)*sz(2)/100];
                typ=[typ,2];
            end
        end
    else
        for i=1:size(P,"r")
            theta=P(i,4)*%pi/180;
            R=[cos(theta),sin(theta);sin(-theta),cos(theta)];
            if (P(i,3))==1 then // explicit_input
                inR=in($,:)*R;
                x=[x,orig(1)+sz(1)-inR(:,1)-P(i,1)*sz(1)/100];
                y=[y,orig(2)+inR(:,2)+P(i,2)*sz(2)/100];
                typ=[typ,1];
            end
            if(P(i,3)==2) then  // implicit
                in2R=in2($,:)*R;
                x=[x,orig(1)+sz(1)-in2R(:,1)-P(i,1)*sz(1)/100];
                y=[y,orig(2)+in2R(:,2)+P(i,2)*sz(2)/100];
                typ=[typ,2];
            end
        end
    end


endfunction
//=========================
function [x,y,typ]=CCS_outputs(o)
    // Copyright INRIA
    xf=60
    yf=40
    [orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
    out=size(o.model.out,1);clkout=size(o.model.evtout,1);
    P=[2,50,1,0; 70,98,2,0;70,2,-2,0]

    in=  [-1 -1; 1  0;-1  1; -1 -1; -1 0]*diag([xf/28,yf/28]) ;// left_triangle
    out= [-1 -1; 1  0;-1  1; -1 -1;  1 0]*diag([xf/28,yf/28]) ;// downward_triangle
    in2= [-1 -1; 1 -1; 1  1; -1  1; -1 -1; 0 0]*diag([xf/28,yf/28])
    out2=[ 1  1;-1  1;-1 -1;  1 -1;  1  1; 0 0]*diag([xf/28,yf/28])

    x=[];y=[];typ=[];
    if orient then
        for i=1:size(P,"r")
            theta=P(i,4)*%pi/180;
            R=[cos(theta),sin(theta);sin(-theta),cos(theta)];
            if (P(i,3))==-1 then // explicit_output
                outR=out($,:)*R;
                x=[x,orig(1)+outR(:,1)+P(i,1)*sz(1)/100];
                y=[y,orig(2)+outR(:,2)+P(i,2)*sz(2)/100];
                typ=[typ,1];
            end
            if(P(i,3)==-2) then  // implicit
                out2R=out2($,:)*R;
                x=[x,orig(1)+out2R(:,1)+P(i,1)*sz(1)/100];
                y=[y,orig(2)+out2R(:,2)+P(i,2)*sz(2)/100];
                typ=[typ,2];
            end
        end
    else
        for i=1:size(P,"r")
            theta=P(i,4)*%pi/180;
            R=[cos(theta),sin(theta);sin(-theta),cos(theta)];
            if (P(i,3))==-1 then // explicit_output
                outR=out($,:)*R;
                x=[x,orig(1)+sz(1)-outR(:,1)-P(i,1)*sz(1)/100];
                y=[y,orig(2)+outR(:,2)+P(i,2)*sz(2)/100];
                typ=[typ,1];
            end
            if(P(i,3)==-2) then  // implicit
                out2R=out2($,:)*R;
                x=[x,orig(1)+sz(1)-out2R(:,1)-P(i,1)*sz(1)/100];
                y=[y,orig(2)+out2R(:,2)+P(i,2)*sz(2)/100];
                typ=[typ,2];
            end
        end
    end

endfunction
