// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->

// Load and run a diagram that calls a Scilab macro (tkscaleblk.sci)
exec("SCI/modules/scicos/tests/unit_tests/bug_8348.cosf", -1);
// Call sciblk2 instead of sciblk4
scs_m.objs(1).model.sim = list("tkscaleblk", 3);
scs_m.props.tf = 100;

// Rewrite tkscaleblk so it minds sciblk2's calling sequence
wMode = warning("query");
warning("off")
function [xd, tvec, z, x, outptr] = tkscaleblk(flag, nevprt, t, x, z, rpar, ipar, inptr)
    outptr = list();
    if flag == 1 then
        // Output update
        slider = get("-38f07e57:12bd41b596e:-7f2b#slider");

        if slider <> [] then
            // Calculate real value
            value = get(slider,"value") / rpar(3);

            w = get("-38f07e57:12bd41b596e:-7f2b");
            if w <> [] then
                set(w, "info_message", string(value));
            end

            outptr = list(value);
        end
    elseif flag == 4 then
        // Initialization

        // If already exists (stopped) then reuse
        f = get("-38f07e57:12bd41b596e:-7f2b");
        if f <> [] then
            return;
        end

        f = figure("Figure_name", "TK Source: " + "", ...
        "dockable", "off", ...
        "infobar_visible" , "on", ...
        "toolbar", "none", ...
        "menubar_visible", "off", ...
        "menubar", "none", ...
        "backgroundcolor", [1 1 1], ...
        "default_axes", "off", ...
        "figure_size", [180 350], ...
        "layout", "border", ...
        "figure_position", [40 40], ...
        "Tag", "-38f07e57:12bd41b596e:-7f2b");

        frame_slider = uicontrol(f, ...
        "style", "frame", ...
        "constraints", createConstraints("border", "left", [180, 0]), ...
        "border", createBorder("line", "lightGray", 1), ...
        "backgroundcolor", [1 1 1], ...
        "layout", "gridbag");

        // Slider
        bounds = rpar(1:2);
        initial = mean(bounds);
        uicontrol(frame_slider, ...
        "Style", "slider", ...
        "Tag", "-38f07e57:12bd41b596e:-7f2b#slider", ...
        "Min", bounds(1), ...
        "Max", bounds(2), ...
        "Value", initial, ...
        "Position", [0 0 10 20], ...
        "SliderStep", [rpar(3) 2*rpar(3)]);

        frame_label = uicontrol(frame_slider, ...
        "style", "frame", ...
        "constraints", createConstraints("border", "right"), ...
        "backgroundcolor", [1 1 1], ...
        "layout", "gridbag");

        // Labels
        labels = string([bounds(2) ; ...
        mean([bounds(2) initial])  ; ...
        initial                    ; ...
        mean([bounds(1) initial])  ; ...
        bounds(1)]);
        labels = "<html>" + strcat(labels, "<br /><br /><br />") + "</html>";

        uicontrol(frame_label, ...
        "Style", "text", ...
        "String", labels(1), ...
        "FontWeight", "bold", ...
        "backgroundcolor", [1 1 1]);

        // Update default value
        outptr = list(initial / rpar(3));
    elseif flag == 5 then
        // Ending
        f = get("-38f07e57:12bd41b596e:-7f2b");
        if f <> [] then
            close(f);
        end
    end
    xd = 0;
    tvec = [];
endfunction
warning(wMode);

cpr = scicos_simulate(scs_m);

cpr229 = [ ...
1;
5.5;
5.5;
5.5];

// Check the sensitive value of the continuous state
assert_checkalmostequal(list2vec(cpr(2)(2)(9)), cpr229);
close(gcf());

// Load and run a diagram that calls a Scilab macro (anim_pen.sci)
exec("SCI/modules/scicos/tests/unit_tests/pendulum_anim5.cosf", -1);
// Call sciblk2 instead of sciblk4
scs_m.objs(8).model.sim = list("anim_pen", 3);

// Rewrite anim_pen so it minds sciblk2's calling sequence
wMode = warning("query");
warning("off")
function [xd, tvec, z, x, outptr] = anim_pen(flag, nevprt, t, x, z, rpar, ipar, inptr)
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
        //** INIT
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
        //** UPDATE
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
    xd = 0;
    tvec = [];
    outptr = list();
endfunction
warning(wMode);

cpr = scicos_simulate(scs_m);

cpr229 = [ ...
-0.00000010072892435; ...
-0.0000000093251865; ...
12.731754622278423; ...
0.00000000775393652; ...
-0.00000007274473145; ...
0.00000008598899216; ...
-0.00000000636625628; ...
-0.00000010072892435;
-0.0000000093251865; ...
-0.00000100269157242; ...
12.7317556249699955; ];

// Check the sensitive value of the continuous state
assert_checkalmostequal(list2vec(cpr(2)(2)(9)), cpr229);
