// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_close(tree)
    // M2SCI function
    // Conversion function for Matlab close()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_close()

    global %graphicswindow

    if tree.lhs(1).name<>"ans" then
        no_equiv(msprintf(gettext("%s will be ignored at run time."),tree.lhs(1).name))
    end

    // close
    if rhs==0 then
        if %graphicswindow==Handle then
            tree.name="delete"
            tree.rhs(1)=Funcall("gcf",1,list(),list())
        else
            // Nothing changed
        end
        // close(h) - close('all') - close(name)
    elseif rhs==1 then
        h=tree.rhs(1)
        if h.vtype==Handle then // close(h)
            tree.name="delete"
        elseif h.vtype==Double then // close(h)
            // Nothing changed
        elseif h.vtype==String then
            if typeof(h)=="cste" then
                if h.value=="all" then // close('all')
                    tree.name="xdel"
                    tree.rhs(1)=Funcall("winsid",1,list(),list())
                else // close(name)
                    tree.name="mtlb_close"
                end
            else
                tree.name="mtlb_close"
            end
        else // Unknown type for h
            tree.name="mtlb_close"
        end
    else // close('all','hidden')
        set_infos(gettext("All windows will be deleted."),2);
        tree.name="xdel"
        tree.rhs(1)=Funcall("winsid",1,list(),list())
        tree.rhs(2)=null()
    end
endfunction
