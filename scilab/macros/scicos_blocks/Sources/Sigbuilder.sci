function [x,y,typ]=Sigbuilder(job,arg1,arg2)
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
  ppath = list(1,3)
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
    if diffobjs(xxn,xx) then 
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
              Title=["Sigbuilder","./"],..
              tol=[ .0001; .000001;1.000E-10;100001;0;0;0],..
              tf=200,..
              context=" ",..
              void1=[],..
              options=tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
              list([5,1],[4,1]),[ .8, .8, .8]),..
              void2=[],..
              void3=[],..
              doc=list()))
scs_m_1.objs(1)=scicos_block(..
                gui="CURVE_c",..
                graphics=scicos_graphics(..
                         orig=[333.41693,565.89722],..
                         sz=[40,40],..
                         flip=%t,..
                         theta=0,..
                         exprs=[],..
                         pin=[],..
                         pout=6,..
                         pein=4,..
                         peout=2,..
                         gr_i=list(["txt=[''CURVE''];";"xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"],..
                         8),..
                         id="",..
                         in_implicit=[],..
                         out_implicit="E"),..
                model=scicos_model(..
                         sim=list("curve_c",4),..
                         in=[],..
                         in2=[],..
                         intyp=1,..
                         out=1,..
                         out2=[],..
                         outtyp=1,..
                         evtin=1,..
                         evtout=1,..
                         state=[],..
                         dstate=[],..
                         odstate=list(),..
                         rpar=[0;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19; -.7616491; .6755537;
                         1.4739763;1.1443051; .8529775; .4529708; .7223316;1.9273332; .6380837;
                          -.8498895; .2546697;-1.5417208; -.6834217; -.7209534; .8145126; .3240162;
                          -.1884803; .4241610;-1.0327357; -.6779672;1.756593;1.026554;0; -.3093156;
                          -.3371249;0; .4402990;0;-1.3815015;0;0;0;0;0;0; -.5012552;0;0;0;1.0643056],..
                         ipar=[20;5;1],..
                         opar=list(),..
                         blocktype="c",..
                         firing=0,..
                         dep_ut=[%f,%t],..
                         label="",..
                         nzcross=0,..
                         nmode=0,..
                         equations=list()),..
                doc=list())
scs_m_1.objs(2)=scicos_link(..
                  xx=[353.41693;354.69887],..
                  yy=[560.18294;551.413],..
                  id="drawlink",..
                  thick=[0,0],..
                  ct=[5,-1],..
                  from=[1,1,0],..
                  to=[3,1,1])
scs_m_1.objs(3)=scicos_block(..
                gui="CLKSPLIT_f",..
                graphics=scicos_graphics(..
                         orig=[354.69887;551.413],..
                         sz=[ .3333333, .3333333],..
                         flip=%t,..
                         theta=0,..
                         exprs=[],..
                         pin=[],..
                         pout=[],..
                         pein=2,..
                         peout=[8;4],..
                         gr_i=list([],8),..
                         id="",..
                         in_implicit=[],..
                         out_implicit=[]),..
                model=scicos_model(..
                         sim="split",..
                         in=[],..
                         in2=[],..
                         intyp=1,..
                         out=[],..
                         out2=[],..
                         outtyp=1,..
                         evtin=1,..
                         evtout=[1;1],..
                         state=[],..
                         dstate=[],..
                         odstate=list(),..
                         rpar=[],..
                         ipar=[],..
                         opar=list(),..
                         blocktype="d",..
                         firing=[%f,%f,%f],..
                         dep_ut=[%f,%f],..
                         label="",..
                         nzcross=0,..
                         nmode=0,..
                         equations=list()),..
                doc=list())
scs_m_1.objs(4)=scicos_link(..
                  xx=[354.69887;304.24048;304.24048;302.05225;353.41693;353.41693],..
                  yy=[551.413;551.413;636.02377;636.02377;636.02377;611.61151],..
                  id="drawlink",..
                  thick=[0,0],..
                  ct=[5,-1],..
                  from=[3,2,0],..
                  to=[1,1,1])
scs_m_1.objs(5)=scicos_block(..
                gui="OUT_f",..
                graphics=scicos_graphics(..
                         orig=[401.98836,575.89722],..
                         sz=[20,20],..
                         flip=%t,..
                         theta=0,..
                         exprs="1",..
                         pin=6,..
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
scs_m_1.objs(6)=scicos_link(..
                  xx=[381.98836;401.98836],..
                  yy=[585.89722;585.89722],..
                  id="drawlink",..
                  thick=[0,0],..
                  ct=[1,1],..
                  from=[1,1,0],..
                  to=[5,1,1])
scs_m_1.objs(7)=scicos_block(..
                gui="CLKOUTV_f",..
                graphics=scicos_graphics(..
                         orig=[344.69887,491.413],..
                         sz=[20,30],..
                         flip=%t,..
                         theta=0,..
                         exprs="1",..
                         pin=[],..
                         pout=[],..
                         pein=8,..
                         peout=[],..
                         gr_i=list(" ",8),..
                         id="",..
                         in_implicit=[],..
                         out_implicit=[]),..
                model=scicos_model(..
                         sim="output",..
                         in=[],..
                         in2=[],..
                         intyp=1,..
                         out=[],..
                         out2=[],..
                         outtyp=1,..
                         evtin=1,..
                         evtout=[],..
                         state=[],..
                         dstate=[],..
                         odstate=list(),..
                         rpar=[],..
                         ipar=1,..
                         opar=list(),..
                         blocktype="d",..
                         firing=[],..
                         dep_ut=[%f,%f],..
                         label="",..
                         nzcross=0,..
                         nmode=0,..
                         equations=list()),..
                doc=list())
scs_m_1.objs(8)=scicos_link(..
                  xx=[354.69887;354.69887],..
                  yy=[551.413;521.413],..
                  id="drawlink",..
                  thick=[0,0],..
                  ct=[5,-1],..
                  from=[3,1,0],..
                  to=[7,1,1])
model=scicos_model(..
         sim="csuper",..
         in=[],..
         in2=[],..
         intyp=1,..
         out=-1,..
         out2=[],..
         outtyp=1,..
         evtin=[],..
         evtout=1,..
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
  //gr_i='xstringb(orig(1),orig(2),''Sigbuilder'',sz(1),sz(2),''fill'')';
  gr_i=["";"if orient then";"  xset(''thickness'',2);";"  xpolys(orig(1)+[0.1;0.21;0.25;0.32;0.39;0.44;0.44;0.6;0.6;0.61;0.65;0.7;0.75;0.86;0.93;0.99;0.99;0.99]*sz(1),orig(2)+[0.58;0.72;0.78;0.81;0.79;0.7;0.46;0.46;0.68;0.74;0.92;0.56;0.71;0.6;0.6;0.48;0.48;0.48]*sz(2),5)";"  xpolys(orig(1)+[0.04,0.1;0.98,0.1]*sz(1),orig(2)+[0.36,0.04;0.36,0.91]*sz(2),[2,2])";"else";"  xset(''thickness'',2);";"  xpolys(orig(1)+[0.9;0.79;0.75;0.68;0.61;0.56;0.56;0.4;0.4;0.39;0.35;0.3;0.25;0.14;0.07;0.01;0.01;0.01]*sz(1),orig(2)+[0.58;0.72;0.78;0.81;0.79;0.7;0.46;0.46;0.68;0.74;0.92;0.56;0.71;0.6;0.6;0.48;0.48;0.48]*sz(2),5)";"  xpolys(orig(1)+[0.96,0.9;0.02,0.9]*sz(1),orig(2)+[0.36,0.04;0.36,0.91]*sz(2),[2,2])";"end";"  xset(''thickness'',1);";"  xset(''color'',1)";"  xfpolys(orig(1)+[0.7;0.63;0.71;0.65;0.57;0.5;0.5;0.7]*sz(1),orig(2)+[0.52;0.49;0.32;0.29;0.46;0.42;0.68;0.52]*sz(2),8)";"  xset(''thickness'',2);";"  xpolys(orig(1)+[0.5;0.7;0.63;0.71]*sz(1),orig(2)+[0.68;0.52;0.49;0.32]*sz(2),1)";"  xstring(orig(1)+0.2*sz(1),orig(2)+0.12*sz(2),""Signal"")";"  xstring(orig(1)+0.2*sz(1),orig(2)+0.0*sz(2),""builder"")";]

  
  x=standard_define([2.3 2.3],model,[],gr_i)
end
endfunction
