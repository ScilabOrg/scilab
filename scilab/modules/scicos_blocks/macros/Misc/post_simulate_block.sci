// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Clement DAVID
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [x,y,typ]=post_simulate_block(job,arg1,arg2)
    x=[];y=[];typ=[];
    select job
    case 'set' then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        
        while %t do
            [ok,fun,exprs]=scicos_getvalue('Set parameters',..
                ['Function name'],..
                list('str',-1),..
                exprs);
            if ~ok then break,end
            if ~isdef(fun) then
                message([_("Error occurred in post_xcos_simulate: Cancelling simulation.");_('Undefined function.')])
                continue
            elseif type(evstr(fun)) <> 13 then
                message([_("Error occurred in post_xcos_simulate: Cancelling simulation.");_('Undefined function.')])
                continue
            end
            // function is already stored into exprs
            break;
        end
    case 'define' then
        model=scicos_model();
        model.sim=list('cdummy', 4)
        exprs=["post_xcos_simulate"]
        
        model.blocktype='d'
        model.dep_ut=[%f %f]
        gr_i=['']
        x=standard_define([6 2],model,exprs,gr_i);
    end
endfunction

