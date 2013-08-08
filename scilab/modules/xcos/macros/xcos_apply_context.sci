// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - INRIA -Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [scs_m,%cpr,needcompile,errmsg]=xcos_apply_context(scs_m)
    // Propagate context through all blocks
    if exists("needcompile")==0 then needcompile=4;end
    errmsg=[];
    [%scicos_context, ierr] = script2var(scs_m.props.context, struct());
    if ierr<>0 then
        ok=%f;%cpr=list()
        errmsg=msprintf(_("%s: Evaluation of the context failed"),"xcos_apply_context");
        return
    end
    [scs_m,%cpr,needcompile,ok] = do_eval(scs_m, struct("state",list()), %scicos_context);
    if ~ok then
        %cpr=list()
        errmsg=msprintf(_("%s: Evaluation of some block parameters failed"),"xcos_apply_context");
        return
    end
endfunction
