function [x,y,typ]=FROMWSB(job,arg1,arg2)
x=[];y=[],typ=[]
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
  //paths to updatable parameters or states
  ppath = list(1)
  newpar=list();
  y=0;
  for path=ppath do
    np=size(path,'*')
    spath=list()
    for k=1:np
      spath($+1)='model'
      spath($+1)='rpar'
      spath($+1)='objs'
      spath($+1)=path(k)
    end
    xx=arg1(spath)// get the block
    execstr('xxn='+xx.gui+'(''set'',xx)')
    if ~isequalbitwise(xxn,xx) then 
      model=xx.model
      model_n=xxn.model
      if ~is_modelica_block(xx) then
        modified=or(model.sim<>model_n.sim)|..
                 ~isequal(model.state,model_n.state)|..
                 ~isequal(model.dstate,model_n.dstate)|..
                 ~isequal(model.odstate,model_n.odstate)|..
                 ~isequal(model.rpar,model_n.rpar)|..
                 ~isequal(model.ipar,model_n.ipar)|..
                 ~isequal(model.opar,model_n.opar)|..
                 ~isequal(model.label,model_n.label)
        if or(model.in<>model_n.in)|or(model.out<>model_n.out)|..
           or(model.in2<>model_n.in2)|or(model.out2<>model_n.out2)|..
           or(model.outtyp<>model_n.outtyp)|or(model.intyp<>model_n.intyp) then
          needcompile=1
        end
        if or(model.firing<>model_n.firing) then
          needcompile=2
        end
        if (size(model.in,'*')<>size(model_n.in,'*'))|..
          (size(model.out,'*')<>size(model_n.out,'*')) then
          needcompile=4
        end
        if model.sim=='input'|model.sim=='output' then
          if model.ipar<>model_n.ipar then
            needcompile=4
          end
        end
        if or(model.blocktype<>model_n.blocktype)|..
           or(model.dep_ut<>model_n.dep_ut) then
          needcompile=4
        end
        if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then
          needcompile=4
        end
        if prod(size(model_n.sim))>1 then
          if model_n.sim(2)>1000 then
            if model.sim(1)<>model_n.sim(1) then
              needcompile=4
            end
          end
        end
      else
        modified=or(model_n<>model)
        eq=model.equations;eqn=model_n.equations;
        if or(eq.model<>eqn.model)|or(eq.inputs<>eqn.inputs)|..
           or(eq.outputs<>eqn.outputs) then
          needcompile=4
        end
      end
     //parameter or states changed
      arg1(spath)=xxn// Update
      newpar(size(newpar)+1)=path// Notify modification
      y=max(y,needcompile)
    end
  end
  x=arg1
  typ=newpar
case 'define' then
scs_m_1=scicos_diagram(..
        version="scicos4.2",..
        props=scicos_params(..
              wpar=[600,450,0,0,600,450],..
              Title=["FROMWSB","./"],..
              tol=[0.0001;0.000001;1.000D-10;100001;0;0;0],..
              tf=10,..
              context=" ",..
              void1=[],..
              options=tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
              list([5,1],[4,1]),[0.8,0.8,0.8]),..
              void2=[],..
              void3=[],..
              doc=list()))
scs_m_1.objs(1)=scicos_block(..
                gui="FROMWS_c",..
                graphics=scicos_graphics(..
                         orig=[348.86041,177.3692],..
                         sz=[60,30],..
                         flip=%t,..
                         theta=0,..
                         exprs=["V","1","1","0"],..
                         pin=[],..
                         pout=4,..
                         pein=2,..
                         peout=2,..
                         gr_i=list(..
                         ["txt=[''From workspace''];";
                         "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"],8),..
                         id="",..
                         in_implicit=[],..
                         out_implicit="E"),..
                model=scicos_model(..
                         sim=list("fromws",4),..
                         in=[],..
                         in2=[],..
                         intyp=1,..
                         out=-1,..
                         out2=1,..
                         outtyp=-1,..
                         evtin=1,..
                         evtout=1,..
                         state=[],..
                         dstate=[],..
                         odstate=list(),..
                         rpar=[],..
                         ipar=[1;-31;1;1;0],..
                         opar=list(),..
                         blocktype="d",..
                         firing=0,..
                         dep_ut=[%f,%f],..
                         label="",..
                         nzcross=0,..
                         nmode=0,..
                         equations=list()),..
                doc=list())
scs_m_1.objs(2)=scicos_link(..
                  xx=[378.86041;378.86041;329.05928;329.05928;378.86041;378.86041],..
                  yy=[171.65492;149.62586;149.62586;221.17448;221.17448;213.08349],..
                  id="drawlink",..
                  thick=[0,0],..
                  ct=[5,-1],..
                  from=[1,1,0],..
                  to=[1,1,1])
scs_m_1.objs(3)=scicos_block(..
                gui="OUT_f",..
                graphics=scicos_graphics(..
                         orig=[437.43183,182.3692],..
                         sz=[20,20],..
                         flip=%t,..
                         theta=0,..
                         exprs="1",..
                         pin=4,..
                         pout=[],..
                         pein=[],..
                         peout=[],..
                         gr_i=list(" ",8),..
                         id="",..
                         in_implicit="E",..
                         out_implicit=[]),..
                model=scicos_model(..
                         sim="output",..
                         in=-1,..
                         in2=-2,..
                         intyp=-1,..
                         out=[],..
                         out2=[],..
                         outtyp=1,..
                         evtin=[],..
                         evtout=[],..
                         state=[],..
                         dstate=[],..
                         odstate=list(),..
                         rpar=[],..
                         ipar=1,..
                         opar=list(),..
                         blocktype="c",..
                         firing=[],..
                         dep_ut=[%f,%f],..
                         label="",..
                         nzcross=0,..
                         nmode=0,..
                         equations=list()),..
                doc=list())
scs_m_1.objs(4)=scicos_link(..
                  xx=[417.43183;437.43183],..
                  yy=[192.3692;192.3692],..
                  id="drawlink",..
                  thick=[0,0],..
                  ct=[1,1],..
                  from=[1,1,0],..
                  to=[3,1,1])
model=scicos_model(..
         sim="csuper",..
         in=[],..
         in2=[],..
         intyp=1,..
         out=-1,..
         out2=[],..
         outtyp=1,..
         evtin=[],..
         evtout=[],..
         state=[],..
         dstate=[],..
         odstate=list(),..
         rpar=scs_m_1,..
         ipar=[],..
         opar=list(),..
         blocktype="h",..
         firing=[],..
         dep_ut=[%f,%f],..
         label="",..
         nzcross=0,..
         nmode=0,..
         equations=list())
  gr_i='xstringb(orig(1),orig(2),''FROMWSB'',sz(1),sz(2),''fill'')';
  x=standard_define([2 2],model,[],gr_i)
end
endfunction
