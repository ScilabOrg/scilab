// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->

// Load and run a diagram that calls a Scilab macro (anim_pen.sci)
exec("SCI/modules/scicos/tests/unit_tests/pendulum_anim5.cosf", -1);
// Call sciblk2 instead of sciblk4
scs_m.objs(8).model.sim = list("anim_pen", 3);

// Rewrite anim_pen so it minds sciblk2's calling sequence
function [xd, t, z, x, y] = anim_pen(flag, nevprt, t, x, z, tvec, rpar, ipar, inptr)
    win=20000+curblock()
    if flag<>4 then
        H=scf(win)
    end
    xold=z
    plen=rpar(1)*1.6;
    csiz=rpar(2)/4;
    phi=rpar(3);
    rcirc=csiz/3;
    if flag==4 then
        xset("window",win)
        set("figure_style","new")
        H=scf(win)
        clf(H)
        Axe=H.children
        Axe.data_bounds=rpar(4:7)
        Axe.isoview="on"

        S=[cos(phi),-sin(phi);sin(phi),cos(phi)]
        XY=S*[rpar(4),rpar(5);-csiz,-csiz]
        xset("color",3)
        xsegs(XY(1,:),XY(2,:)-rcirc)

        xTemp=0;
        theta=0;
        x1=xTemp-csiz;
        x2=xTemp+csiz;
        y1=-csiz;
        y2=csiz
        XY=S*[x1 x2 x2 x1 x1;y1,y1,y2,y2,y1]
        xset("color",5)
        xfpoly(XY(1,:),XY(2,:))// cart
        xset("color",2)
        xfarc(XY(1,1),XY(2,1),rcirc,rcirc,0,360*64) //wheel
        xfarc(XY(1,2),XY(2,2),rcirc,rcirc,0,360*64) //wheel

        XY=S*[xTemp,xTemp+plen*sin(theta);0,0+plen*cos(theta)]//pendulum
        xset("color",2)
        xsegs(XY(1,:),XY(2,:))

    elseif flag==2 then
        Axe=H.children
        xTemp=inptr(1)(1)
        theta=inptr(2)(1)
        drawlater();
        XY=Axe.children(4).data'+ [cos(phi)*(xTemp-xold);sin(phi)*(xTemp-xold)]*ones(1,5)
        Axe.children(4).data=XY'

        Axe.children(3).data(1)=XY(1,1)
        Axe.children(3).data(2)=XY(2,1)
        XY=Axe.children(4).data'+ [cos(phi)*(xTemp-xold-rcirc);sin(phi)*(xTemp-xold-rcirc)]*ones(1,5)
        Axe.children(2).data(1)=XY(1,2)
        Axe.children(2).data(2)=XY(2,2)
        x1=xTemp*cos(phi);
        y1=xTemp*sin(phi)
        XY=[x1,x1+plen*sin(theta);y1,y1+plen*cos(theta)]
        Axe.children(1).data=XY'
        drawnow();
        z=xTemp
    end
    xd = 0
    y = list()
endfunction

cpr = scicos_simulate(scs_m);

// Check the sensitive value of the continuous state
//assert_checkalmostequal(cpr(2)(2)(9)(4), 5.49956686330299);
