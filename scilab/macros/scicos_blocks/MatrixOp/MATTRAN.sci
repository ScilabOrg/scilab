function [x,y,typ]=MATTRAN(job,arg1,arg2)
//
// Copyright INRIA
x=[];y=[];typ=[];
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
  x=arg1
case 'define' then
  model=scicos_model()
  model.sim=list('mattran_m',4)

  model.in=-1
  model.in2=-2
  model.out=-2
  model.out2=-1
  model.dep_ut=[%t %f]

  label=[]
  gr_i=['xstringb(orig(1),orig(2),[''MATTRAN''],sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction

