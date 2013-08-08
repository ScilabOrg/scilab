// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - INRIA -Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [ports,errmsg]=xcos_toplevel_ports(scs_m)
    //This function collects the top level ports of the given diagram
    //The returned variables are empty or two column arrays.
    //   The first column contains the port number
    //   The second column contains the block port index in the scs_m.objs list
    //   The variables are sorted by increasing values of  port numbers
    ports=tlist(["xcsprt","IN","OUT","CLKIN","CLKOUT","INIMPL","OUTIMPL"],...
    [],[],[],[],[],[]);

    IN=[];OUT=[];CLKIN=[];CLKOUT=[];INIMPL=[];OUTIMPL=[];
    errmsg=[]
    for i=1:length(scs_m.objs)
        Oi=scs_m.objs(i)
        if typeof(Oi)=="Block" then
            gui_name=Oi.gui;
            if gui_name=="IN_f" then
                IN=[IN;[Oi.model.ipar,i]];
            elseif gui_name=="OUT_f" then
                OUT=[OUT;[Oi.model.ipar,i]];
            elseif gui_name=="INIMPL_f" then
                INIMPL=[INIMPL;[Oi.model.ipar,i]];
            elseif gui_name=="OUTIMPL_f" then
                OUTIMPL=[OUTIMPL;[Oi.model.ipar,i]];
            elseif or(gui_name==["CLKIN_f" "CLKINV_f"]) then
                CLKIN=[CLKIN;[Oi.model.ipar,i]];
            elseif or(gui_name==["CLKOUT_f" "CLKOUTV_f"]) then
                CLKOUT=[CLKOUT;[Oi.model.ipar,i]];
            end
        end
    end
    IN=gsort(IN,"lr","i");
    nin=size(IN,1) //number of input ports
    if or(IN(:,1)<>(1:nin)') then
        errmsg=_("Input ports are not numbered properly.")
        return
    end

    OUT=gsort(OUT,"lr","i");
    nout=size(OUT,1); //number of output ports
    if or(OUT(:,1)<>(1:nout)') then
        errmsg=_("Output ports are not numbered properly.")
        return
    end

    INIMPL=gsort(INIMPL,"lr","i");
    ninimpl=size(INIMPL,1) //number of input ports
    if or(INIMPL(:,1)<>(1:ninimpl)') then
        errmsg=_("Implicit input ports are not numbered properly.")
        return
    end

    OUTIMPL=gsort(OUTIMPL,"lr","i");
    noutimpl=size(OUTIMPL,1) //number of input ports
    if or(OUTIMPL(:,1)<>(1:noutimpl)') then
        errmsg=_("Implicit ouyput ports are not numbered properly.")
        return
    end

    CLKIN=gsort(CLKIN,"lr","i");
    ninclk=size(CLKIN,1) //number of input ports
    if or(CLKIN(:,1)<>(1:ninclk)') then
        errmsg=_("Activation input ports are not numbered properly.")
        return
    end

    CLKOUT=gsort(CLKOUT,"lr","i");
    noutclk=size(CLKOUT,1) //number of input ports
    if or(CLKOUT(:,1)<>(1:noutclk)') then
        errmsg=_("Activation output ports are not numbered properly.")
        return
    end
    ports=tlist(["xcsprt","IN","OUT","CLKIN","CLKOUT","INIMPL","OUTIMPL"],...
    IN,OUT,CLKIN,CLKOUT,INIMPL,OUTIMPL);
endfunction
