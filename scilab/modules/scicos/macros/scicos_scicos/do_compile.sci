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

function  [%cpr,ok] = do_compile(scs_m)
    // Load the block libs if not defined
    prot = funcprot();
    funcprot(0);
    if ~exists("scicos_diagram") then
        loadXcosLibs();
    end
    funcprot(prot);
    [modelica_libs, scicos_pal_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables();

    //**---- prepare from and to workspace stuff ( "From workspace" block )
    scicos_workspace_init()

    //** extract tolerances from scs_m.props.tol
    tolerances = scs_m.props.tol ;
    //** extract solver type from tolerances
    solver = tolerances(6) ;
    //** initialize a "scicos_debug_gr" variable
    %scicos_debug_gr = %f;

    ////////////////////////////////////////////////////////////////
    // Add global environment variable so that scicos is not lost //
    ////////////////////////////////////////////////////////////////
    needcompile = 4;
    %state0     = list();
    needcompile = 4;
    curwin      = 1000;
    %cpr        = struct();
    %tcur       = 0;
    %cpr.state  = %state0;
    alreadyran = %f;

    tf          = scs_m.props.tf;
    %zoom       = 1.4;
    Select      = [];

    //** extract tolerances from scs_m.props.tol
    tolerances = scs_m.props.tol ;
    //** extract solver type from tolerances
    solver = tolerances(6) ;

    // Propagate context through all blocks
    %scicos_context = struct();
    context = scs_m.props.context;
    //** context eval here
    [%scicos_context, ierr] = script2var(context, %scicos_context);

    // For backward compatibility for scifunc
    if ierr==0 then
        %mm = getfield(1,%scicos_context)
        for %mi=%mm(3:$)
            ierr = execstr(%mi+"=%scicos_context(%mi)","errcatch")
            if ierr<>0 then
                break; //** in case of error exit
            end
        end
    end
    // End of for backward compatibility for scifuncpagate context values

    [scs_m,%cpr,needcompile,ok] = do_eval(scs_m, %cpr, %scicos_context);
    if ~ok then
        msg = msprintf(gettext("%s: Error during block parameters evaluation.\n"), "Xcos");
        messagebox(msg, "Xcos", "error");
        error(msprintf(gettext("%s: Error during block parameters evaluation.\n"), "xcos_simulate"));
    end

    //** update parameters or compilation results
    [%cpr,%state0_n,needcompile,alreadyran,ok] = do_update(%cpr,%state0,needcompile)
    if ~ok then
        error(msprintf(gettext("%s: Error during block parameters update.\n"), "xcos_simulate"));
    end

    clear alreadyran, %state0_n, needcompile;

endfunction
