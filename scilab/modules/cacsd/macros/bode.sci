// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  1985-2010 - INRIA - Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=bode(varargin)
    rhs=size(varargin)

    if rhs == 0 then
        s=poly(0,"s")
        h1=syslin("c",(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01))
        num=22801+4406.18*s+382.37*s^2+21.02*s^3+s^4;
        den=22952.25+4117.77*s+490.63*s^2+33.06*s^3+s^4
        h2=syslin("c",num/den);

        bode([h1;h2],0.01,100,["h1";"h2"])
        return;
    end

    rad = %f;
    asymp = %f;
    if type(varargin($))==10 then
        if or(varargin($) == ["rad" "asymp" "radasymp"]) then
            select varargin($)
            case "rad" then
                rad = %t;
            case "asymp" then
                asymp = %t;
            else // "radasymp"
                rad = %t;
                asymp = %t;
            end
            if type(varargin($-1))==10 then
                comments=varargin($-1),rhs=rhs-1;
            else
                comments=[];
            end
        else
            comments=varargin($),rhs=rhs-1;
        end
    else
        comments=[];
    end
    fname="bode";// for error messages
    fmax=[];
    discr=%f; // for shannon limit
    if or(typeof(varargin(1))==["state-space" "rational"]) then
        // sys,fmin,fmax [,pas] or sys,frq
        refdim=1 // for error message
        discr=varargin(1).dt<>"c";
        if rhs==1 then // sys
            [frq,repf]=repfreq(varargin(1),1d-3,1d3)
        elseif rhs==2 then // sys,frq or sys, radasymp
            if size(varargin(2),2)<2 & rad == %f & asymp == %f then
                error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),..
                fname,2,1))
            end
            if rad == %f & asymp == %f then
                [frq,repf]=repfreq(varargin(1:rhs))
            else
                [frq,repf]=repfreq(varargin(1:rhs-1))
            end
        elseif or(rhs==(3:5)) then // sys,fmin,fmax [,pas] [,radasymp]
            if rad == %f & asymp == %f then
                [frq,repf]=repfreq(varargin(1:rhs))
            else
                [frq,repf]=repfreq(varargin(1:rhs-1))
            end
        else
            error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,1,5))
        end
        [phi,d]=phasemag(repf)
        if rhs>=3 then fmax=varargin(3),end
    elseif type(varargin(1))==1 then
        // frq,db,phi [,comments] or frq, repf [,comments]
        refdim=2
        select rhs
        case 2 then // frq,repf
            frq=varargin(1);
            if size(frq,2)<2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),..
                fname,1,1))
            end
            if size(frq,2)<>size(varargin(2),2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
                fname,1,2))
            end
            [phi,d]=phasemag(varargin(2))
        case 3 then  // frq,db,phi or frq,db,radasymp
            if rad == %f & asymp == %f then
                [frq,d,phi]=varargin(1:rhs)
            else
                [frq,d,phi]=varargin(1:rhs-1)
            end
            if size(frq,2)<>size(d,2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
                fname,1,2))
            end
            if size(frq,2)<>size(phi,2) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
                fname,1,3))
            end
        case 4 then // frq,db,phi,radasymp
            if rad == %t | asymp == %t then
                [frq,d,phi]=varargin(1:rhs-1)
            end
        else
            error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,2,4))
        end
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system or row vector of floats expected.\n"),fname,1))
    end;
    frq=frq';d=d',phi=phi'
    [n,mn]=size(d)

    if comments==[] then
        hx=0.48
    else
        if size(comments,"*")<>mn then
            error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of elements expected.\n"),...
            fname,refdim,rhs+1))
        end
        hx=0.43
    end;

    fig=gcf();
    immediate_drawing=fig.immediate_drawing;
    fig.immediate_drawing="off";

    sciCurAxes=gca();
    axes=sciCurAxes;
    wrect=axes.axes_bounds;


    // magnitude
    axes.axes_bounds=[wrect(1)+0,wrect(2)+0,wrect(3)*1.0,wrect(4)*hx*0.95]
    axes.data_bounds = [min(frq),min(d);max(frq),max(d)];
    axes.log_flags = "lnn" ;
    axes.grid=color("lightgrey")*ones(1,3);
    axes.axes_visible="on";
    axes.clip_state = "clipgrf";
    if size(d,2)>1&size(frq,2)==1 then
        xpolys(frq(:,ones(1,mn)),d,1:mn)
    else
        xpolys(frq,d,1:mn)
    end
    // set datatips info
    e=gce();

    for i=1:size(e.children,"*")
        datatipInitStruct(e.children(i),"formatfunction","formatBodeMagTip")
    end

    if discr&fmax<>[]&max(frq)<fmax then
        xpoly(max(frq)*[1;1],axes.y_ticks.locations([1 $]));e=gce();
        e.foreground=5;
    end
    xtitle("",_("Frequency (Hz)"),_("Magnitude (dB)"));

    // phase
    axes=newaxes();
    axes.axes_bounds=[wrect(1)+0,wrect(2)+wrect(4)*hx,wrect(3)*1.0,wrect(4)*hx*0.95];
    axes.data_bounds = [min(frq),min(phi);max(frq),max(phi)];
    axes.log_flags = "lnn" ;
    axes.grid=color("lightgrey")*ones(1,3);
    axes.axes_visible="on";
    axes.clip_state = "clipgrf";
    if size(phi,2)>1&size(frq,2)==1 then
        xpolys(frq(:,ones(1,mn)),phi,1:mn)
    else
        xpolys(frq,phi,1:mn)
    end
    ephi=gce()
    // set datatips info
    for i=1:size(ephi.children,"*")
        datatipInitStruct(ephi.children(i),"formatfunction","formatBodePhaseTip")
    end

    if discr&fmax<>[]&max(frq)<fmax then
        xpoly(max(frq)*[1;1],axes.y_ticks.locations([1 $]));e=gce();
        e.foreground=5;
    end
    xtitle("",_("Frequency (Hz)"),_("Phase (degree)"));
    // create legend
    if comments<>[] then
        c=captions(ephi.children,comments,"lower_caption")
        c.background=get(gcf(),"background")
    end
    fig.immediate_drawing=immediate_drawing;
    // return to the previous scale
    set( "current_axes", sciCurAxes ) ;

    if rad == %t then
        bode_Hz2rad_2(fig);
    end
    if asymp == %t then
        if size(varargin(1), "*") <> 1 then
            first = varargin(1);
            bode_asymp(first(1, 1), min(frq), max(frq));
        else
            bode_asymp(varargin(1), min(frq), max(frq));
        end
    end

endfunction

function str=formatBodeMagTip(curve,pt,index)
    //this function is called by the datatips mechanism to format the tip
    //string for the magnitude bode curves
    str=msprintf("%.4g"+_("Hz")+"\n%.4g"+_("dB"), pt(1),pt(2))
endfunction

function str=formatBodePhaseTip(curve,pt,index)
    //this function is called by the datatip mechanism to format the tip
    //string for the bode phase curves
    str=msprintf("%.4g"+_("Hz")+"\n %.4g"+"°", pt(1),pt(2))
endfunction

function []=bode_Hz2rad_2(h)
    // This function modifies the Bode diagrams for a rad/s display instead of Hz.
    // h is a hanlde of a figure containing Bode diagrams.
    // Ref: http://forge.scilab.org/index.php/p/cpge/source/tree/HEAD/macros/bode_Hz2rad_2.sci

    // k=1 phase, k=2 gain
    labels=["Phase (Â°)";"Amplitude (dB)"]
    pos_h=[9,5];
    for k=1:2
        for i=1:size(h.children(k).children,1)
            if(h.children(k).children(i).type=="Compound")
                for j=1:size(h.children(k).children(i).children,1)
                    h.children(k).children(i).children(j).data(:,1)=h.children(k).children(i).children(j).data(:,1)*2*%pi;
                end

                // h.children(k).title.text=h.children(k).y_label.text;
                xmin1=h.children(k).data_bounds(1)*2*%pi;
                xmax1=h.children(k).data_bounds(2)*2*%pi;
                ymin1=h.children(k).data_bounds(3);
                ymax1=h.children(k).data_bounds(4);

                rect=[xmin1,ymin1,xmax1,ymax1]
                nb_dec=log(xmax1/xmin1)/log(10);
                h.children(k).x_label.text="Pulsation (rad/s)"
                h.children(k).x_location="bottom";
                h.children(k).y_label.text=labels(k);
                replot(rect,h.children(k))
            end
        end
    end

endfunction

function []=bode_asymp(ss,w_min,w_max)
    // This function plots asymptotes for the magnitude and phase graphs.
    // Ref: http://forge.scilab.org/index.php/p/cpge/source/tree/HEAD/macros/REP_FREQ_pre_simulate.sci
    // "ss" is a linear system given by its transfer function representation and defined by syslin.
    h = ss;
    if typeof(h) == "state-space" then
        h = ss2tf(ss); // "ss" is a linear system given by its state space representation and defined by syslin.
    end

    root_num = roots(h.num);
    root_den = roots(h.den);

    if (find(root_num==0))
        disp("Problem class of system is negative")
    end
    rac_nul = find(root_den==0)
    alpha = length(rac_nul)
    s=poly(0,"s");
    try K = horner(h*s^alpha, 0); catch error(msprintf(_("%s: Problem evaluating the first argument.\n"),"bode")); end

    root_den(rac_nul)=[]; // Removing the zeros

    if (length([find(abs(root_num)>1e8) find(abs(root_num)<1e-8)])>0) then
        disp("Extreme root removed : "+string(root_num([find(abs(root_num)>1e8) find(abs(root_num)<1e-8)])'))
        root_num([find(abs(root_num)>1e8) find(abs(root_num)<1e-8)])=[]
    end
    if (length([find(abs(root_den)>1e8) find(abs(root_den)<1e-8)])>0) then
        disp("Extreme root removed : "+string(root_den([find(abs(root_den)>1e8) find(abs(root_den)<1e-8)])'))
        root_den([find(abs(root_den)>1e8) find(abs(root_den)<1e-8)])=[]
    end

    i=1;
    puls = [];
    order = [];
    while i <= length(root_num) // Real root
        if (isreal(root_num(i),0)) then
            puls = [puls -root_num(i)];
            order = [order 1];
            i=i+1;
        else // Complex root
            xi=1/sqrt(1+(imag(root_num(i))/real(root_num(i)))^2);
            puls = [puls -real(root_num(i))/xi];
            i=i+2;
            order = [order 2];
        end
    end
    i=1;
    while i <= length(root_den) // Real root
        if (isreal(root_den(i),0)) then
            puls = [puls -root_den(i)];
            order = [order -1];
            i=i+1;
        else // Complex root
            xi=1/sqrt(1+(imag(root_den(i))/real(root_den(i)))^2);
            puls = [puls -real(root_den(i))/xi];
            order = [order -2];
            i=i+2;
        end
    end

    [puls,ind]=gsort(puls,"g","i")
    order = order(ind)

    asym = [-20*alpha];
    phas = [-90*alpha];
    i=1;
    while(i<=length(puls))
        new_dir = asym($)+order(i)*20;
        asym = [asym new_dir]
        new_arg = phas($)+order(i)*90;
        phas = [phas new_arg];
        i=i+1
    end

    // bode(h,w_min,w_max)
    fig=gcf()
    sca(fig.children(2))
    wmin=w_min
    wmax=w_max
    puls_to_plot=[]

    for p=real(puls)
        if p>=wmin & p<=wmax then
            puls_to_plot($+1)=p
        end
    end
    puls=[wmin puls_to_plot' wmax];
    // End change DV

    eq_asymp = [20*log10(K/wmin^alpha)];
    puls_p=[];
    phas($+1)=phas($);
    eq_phas =[phas(1)];
    i=2
    while (i<=length(puls))
        eq_asymp = [eq_asymp eq_asymp($)+asym(i-1)*log10(puls(i)/puls(i-1))]

        puls_p=[puls_p puls(i-1) puls(i)]
        eq_phas =[eq_phas phas(i-1) phas(i)]
        i=i+1
    end

    plot(puls,eq_asymp,"b--")

    sca(fig.children(1))
    plot(puls_p,eq_phas(1:$-1),"r--")

endfunction
