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
              tf=100,..
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
                         orig=[332.88977,584.45066],..
                         sz=[40,40],..
                         flip=%t,..
                         theta=0,..
                         exprs=["3";"[0 1 3*%pi 12]";"[1 1 8 4]";"y";"n"],..
                         pin=[],..
                         pout=6,..
                         pein=4,..
                         peout=2,..
                         gr_i=list(..
                         ["rpar=arg1.model.rpar;n=model.ipar(1);order=model.ipar(2);";
                         "xx=rpar(1:n);yy=rpar(n+1:2*n);NP=6;";
                         "[XX,YY,rpardummy]=Do_Spline(n,order,xx,yy,NP)";
                         "xmx=maxi(XX);xmn=mini(XX);";
                         "ymx=maxi(YY);ymn=mini(YY);";
                         "dx=xmx-xmn;if dx==0 then dx=maxi(xmx/2,1);end";
                         "xmn=xmn-dx/20;xmx=xmx+dx/20;";
                         "dy=ymx-ymn;if dy==0 then dy=maxi(ymx/2,1);end;";
                         "ymn=ymn-dy/20;ymx=ymx+dy/20;";
                         "xx2=orig(1)+sz(1)*((XX-xmn)/(xmx-xmn));";
                         "yy2=orig(2)+sz(2)*((YY-ymn)/(ymx-ymn));";
                         "xset(''color'',2)";
                         "xpoly(xx2,yy2,''lines'');"],8),..
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
                         rpar=[0;1;9.424778;12;1;1;8;4; -.3276272; .3022189; -.4438581;-2.8311723],..
                         ipar=[4;3;1],..
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
                  xx=[352.88977;350.83491],..
                  yy=[578.73637;552.34327],..
                  id="drawlink",..
                  thick=[0,0],..
                  ct=[5,-1],..
                  from=[1,1,0],..
                  to=[3,1,1])
scs_m_1.objs(3)=scicos_block(..
                gui="CLKSPLIT_f",..
                graphics=scicos_graphics(..
                         orig=[350.83491;552.34327],..
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
                  xx=[350.83491;286.24528;286.24528;352.16095;352.88977],..
                  yy=[552.34327;552.34327;661.27274;661.27274;630.16494],..
                  id="drawlink",..
                  thick=[0,0],..
                  ct=[5,-1],..
                  from=[3,2,0],..
                  to=[1,1,1])
scs_m_1.objs(5)=scicos_block(..
                gui="OUT_f",..
                graphics=scicos_graphics(..
                         orig=[401.4612,594.45066],..
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
                  xx=[381.4612;401.4612],..
                  yy=[604.45066;604.45066],..
                  id="drawlink",..
                  thick=[0,0],..
                  ct=[1,1],..
                  from=[1,1,0],..
                  to=[5,1,1])
scs_m_1.objs(7)=scicos_block(..
                gui="CLKOUTV_f",..
                graphics=scicos_graphics(..
                         orig=[340.83491,492.34327],..
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
                  xx=[350.83491;350.83491],..
                  yy=[552.34327;522.34327],..
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
//  gr_i='xstringb(orig(1),orig(2),''Sigbuilder'',sz(1),sz(2),''fill'')';
    gr_i=['ipar=arg1.model.rpar.objs(1).model.ipar';
	'rpar=arg1.model.rpar.objs(1).model.rpar';
	'n=ipar(1);order=ipar(2);';
	'xx=rpar(1:n);yy=rpar(n+1:2*n);NP=6;';
	'[XX,YY,rpardummy]=Do_Spline2(n,order,xx,yy,NP)';
	'xmx=maxi(XX);xmn=mini(XX);';
	'ymx=maxi(YY);ymn=mini(YY);';
	'dx=xmx-xmn;if dx==0 then dx=maxi(xmx/2,1);end';
	'xmn=xmn-dx/20;xmx=xmx+dx/20;';
	'dy=ymx-ymn;if dy==0 then dy=maxi(ymx/2,1);end;';
	'ymn=ymn-dy/20;ymx=ymx+dy/20;';
	'rect=[xmn,ymn;xmx,ymx];';
	'xx2=orig(1)+sz(1)*((XX-xmn)/(xmx-xmn));';
	'yy2=orig(2)+sz(2)*((YY-ymn)/(ymx-ymn));';
	'xset(''color'',2)';
	'xpoly(xx2,yy2,''lines'');']
    
  x=standard_define([2 2],model,[],gr_i)
end
endfunction


//=========================================================
function [X,Y,orpar]=Do_Spline2(N,order,x,y,NP)

  X=[];Y=[];orpar=[];
  for i=1:N-1
    X=[X;linspace(x(i),x(i+1),NP)']; // pour tous sauf "linear" et "zero order"
  end

  METHOD=getmethod(order);

  if (METHOD=='zero order') then 
    X=x(1);Y=y(1);
    for i=1:N-1
      X=[X;x(i);x(i+1);x(i+1)];
      Y=[Y;y(i);y(i);y(i+1)];
    end  
  end    
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='linear') then
    X=[];
    for i=1:N
      X=[X;x(i)];
      Y=[Y;y(i)];
    end  
  end
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (N>2) & (METHOD=='order 2') then
    Z=ORDER2(x,y);
    A=Z(1:N-1); 
    B=Z(N:2*N-2);
    C=Z(2*N-1:3*N-3);
    
    for j=1:size(X,'*')
      for i=N-1:-1:1
	if X(j)>=x(i) then,break;end
      end
      Y(j)=A(i)*(X(j)-x(i))^2+B(i)*(X(j)-x(i))+C(i);
    end    
    orpar=matrix(Z,-1,1)   
  end  
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='not_a_knot') then
    try
    d = splin(x, y, METHOD);
    Y = interp(X, x, y, d);    
    orpar=d(:);
    catch
     xinfo('ERROR in SPLINE: '+METHOD)
    end
    
  end
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='periodic') then
    if y(1)<>y(N) then 
      y(N)=y(1)
    end
    try 
      d = splin(x, y,METHOD);
      Y = interp(X, x, y, d);  
      orpar=d(:);
    catch
    xinfo('ERROR in SPLINE: '+METHOD)
    end
  end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='monotone' ) then
    try
      d = splin(x, y, METHOD);
      Y = interp(X, x, y, d);  
      orpar=d(:);
    catch
    xinfo('ERROR in SPLINE: '+METHOD)
    end
  
  end
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='fast') then
    try
      d = splin(x, y, METHOD);
      Y = interp(X, x, y, d);    
      orpar=d(:);
    catch
      xinfo('ERROR in SPLINE:  '+METHOD)    
    end  
  end  
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='clamped') then
    try
      d = splin(x, y, METHOD,[0;0]);
      Y = interp(X, x, y, d);    
      orpar=d(:);
    catch
      xinfo('ERROR in SPLINE: '+METHOD)    
    end
  end  
  
endfunction

function METHOD=getmethod(order)
  select order
   case 0 then, METHOD='zero order'
   case 1 then, METHOD='linear'
   case 2 then, METHOD='order 2'
   case 3 then, METHOD='not_a_knot'
   case 4 then, METHOD='periodic'
   case 5 then, METHOD='monotone'
   case 6 then, METHOD='fast'
   case 7 then, METHOD='clamped'
  end
endfunction






