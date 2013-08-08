// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - INRIA -Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function num=console_message(varargin)
    //Non interactive message function used to overload the message and messagebox
    //functions during the xcos_compile process when used in batch mode
    //waiting for a better handling of interactive/non interactive mode in c_pass1,c_pass2,...
    num=0
    mprintf("%s\n",varargin(1))
endfunction
function console_path(path,mess,with_intermediates)
    //Non interactive path and message function used to overload the hilite_path
    //functions during the xcos_compile process when used without xcos window
    //waiting for a better handling of interactive/non interactive mode in c_pass1,c_pass2,...

    // retrieve the root diagram
    if isdef("scs_m_s") then
        diagram = scs_m_s;
    elseif isdef("scs_m") then
        diagram = scs_m;
    else
        error("scs_m variable does not exist");
    end
    uid = []; // empty uid path

    for i = 1:length(path)
        o = diagram.objs(path(i));
        if typeof(o) == "Block" then
            // masked superblock case
            if isempty(o.doc) then
                // we abort because blocks do not have uuid inside
                // masked superblocks
                break;
            end
            // normal case
            uid=[uid o.doc(1)+""];

            if o.model.sim == "super" then
                diagram = o.model.rpar;
            else
                break;
            end
        end
    end

    path=strcat(string(path),"->")
    uid=strcat(uid,"->")

    mprintf("%s\n",mess)
    mprintf("implied block path and uids:")
    mprintf("  %s\n  %s\n",path,uid)
endfunction
