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

function  [%cpr,ok] = xcos_compile(scs_m,fname)
    //Prepare and compile an Xcos diagram
    //  scs_m     : the diagram data structure
    //  fname     : character string use for tagging error messages.
    //              Default value: "xcos_compile"
    //  comp_info : a tlist with fields cpr, InPtr, OutPtr
    //              InPtr is the array of index of super bloc inputs in outtb
    //              OutPtr is the array of index of super bloc outputs in outtb


    if argn(2)<2  then fname="xcos_compile";end
    if typeof(scs_m)<>"diagram" then
        error(msprintf(_("%s: Wrong type for input argument #%d: A diagram expected.\n"),...
        fname, 1));
    end

    ////////////////////////////////////////////////////////////////
    // Add global environment variable so that scicos is not lost //
    ////////////////////////////////////////////////////////////////

    needcompile = 4;
    curwin      = 1000;
    %cpr        = struct("state",list());
    %tcur       = 0;
    tf          = scs_m.props.tf;
    alreadyran  = %f;

    if alreadyran then
        // terminate current simulation
        do_terminate();
    end

    if ~exists("%scicos_solver") then
        %scicos_solver = 0 ;
    end

    if ~exists("%scicos_debug_gr") then
        %scicos_debug_gr = %f; //** debug mode : default is "%f"
    end

    // modelica_libs needed to compile Modelica blocks
    if ~exists("modelica_libs") then
        modelica_libs = getModelicaPath();
    end
    [scs_m,%cpr,needcompile,errmsg]=xcos_apply_context(scs_m)
    if ~isempty(errmsg) then
        message(errmsg)
        ok=%f;%cpr=list()
        return
    end

    //Get and check top level input and output ports
    [ports,errmsg]=xcos_toplevel_ports(scs_m);
    if ~isempty(errmsg) then
        ok=%f;%cpr=list()
        message(errmsg)
        return
    end
    if ports.INIMPL<>[]|ports.OUTIMPL<>[] then
        message(_("Implicit ports are not allowed."))
        ok=%f;%cpr=list()
        return
    end

    //Rename top level ports interface and simulation functions
    // bidon is a simulation function that does nothing.
    //This is done to avoid c_pass1 throwing an error.
    //This part of the code should be done only for steadycos, lincos or codegen.
    for i=ports.IN(:,2)';
        scs_m.objs(i).gui="INPUTPORT";
        scs_m.objs(i).model.sim(1)="bidon";
    end
    for i=ports.OUT(:,2)';
        scs_m.objs(i).gui="OUTPUTPORT";
        scs_m.objs(i).model.sim(1)="bidon";
    end

    //** First PASS
    [bllst,connectmat,clkconnect,cor,corinv,ok] = c_pass1(scs_m);
    if ~ok then
        %cpr=list()
        return ;
    end

    if size(connectmat,2)==6 then
        connectmat = connectmat(:,[1 2 4 5])
    end

    //** Second PASS
    %cpr = c_pass2(bllst,connectmat,clkconnect,cor,corinv);
    if %cpr==list() then
        ok = %f ;
    end
    %cpr.ports=ports

endfunction
