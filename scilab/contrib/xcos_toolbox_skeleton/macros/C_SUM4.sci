// This file is part of the Xcos Toolbox Skeleton toolbox
// 
// Copyright (C) 2011 - DIGITEO - Clément DAVID <clement.david@scilab.org>
// see license.txt for more licensing informations

function [x,y,typ]=C_SUM4(job,arg1,arg2)
  x=[];y=[];typ=[]
  select job
   case 'plot' then
    standard_draw(arg1)
   case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
   case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
   case 'getorigin' then
    [x,y]=standard_origin(arg1)
   case 'set' then
    x=arg1;

    // no parameters yet
    [lineCall, stackCall]=where()
    messagebox("No parameters yet.", stackCall(1), "error");
   case 'define' then
    model=scicos_model()
    model.sim=list('sumblk4_tbx',4)
    model.in=[1;1]
    model.intyp=1
    model.out=1
    model.outtyp=1
    model.blocktype='c'
    model.dep_ut=[%f %f]
    
    exprs=[string([])]
    [lineCall, stackCall]=where()
    gr_i=[("txt=[''" + stackCall(1) +"''];") ;..
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"]
    
    x=standard_define([2 2],model,exprs,gr_i)
  end
endfunction

