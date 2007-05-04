//ZBUF_c Scicos discrete memory bloc block
//Classical interface function ver1.0 - scilab-4.1
//20 mars 2007 - INRIA - Author : A.Layec
function [x,y,typ]=ZBUF_c(job,arg1,arg2)
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
   graphics=arg1.graphics;model=arg1.model;
   exprs=graphics.exprs;

   while %t do
      [ok,nu,nz,ev,varnam,ci_cnt,herit,exprs]=getvalue('Scicos circulate buffer block',...
      ['Size of inputs';
       'Size of buffer';
       'Store event values (no:0, yes:1)';
       'Global scilab variable name';
       'Initial counter value';
       'Inherit (no:0, yes:1)'],...
      list('vec',-1,'vec',1,'vec',1,'str',1,'vec',1,'vec',1),exprs);

      if ~ok then break,end;
      if(nz==0) then
        message("Size of inital buffer state must be positive");
        ok=%f
      elseif(ci_cnt>=nz) then
        message("Initial counter value must be less "+...
                "than the size of the buffer");
        ok=%f
      end
      for i=1:size(nu,'*')
        if(nu(i)>nz) then
          message("Size of input must be less or equal "+...
                  "of the size of the buffer");
          ok=%f
          break;
        end
        if(nu(i)<>nu(1)) then
          message("All input sizes must be equal");
          ok=%f
          break;
        end
      end
      if length(varnam)<>0 then
        ok=check_varname(varnam)
        if ~ok then
          message(["Invalid variable name.";
                   "Please choose another variable name."] );
        else
          ok=check_varbuf(varnam)
          if ~ok then
            message(["Another block use this variable.";
                     "Please choose another variable name."] );
          end
        end
      end

      if ok then
        if herit==1 then
          [model,graphics,ok]=check_io(model,graphics,nu,[],[],[])
        else
          [model,graphics,ok]=check_io(model,graphics,nu,[],1,[])
        end
        lz=nz*size(nu,'*');
        if ev<>1 then ev=0; end
        if ev==1 then lz=lz+nz; end
        if length(varnam)<>0 then //sci_buf
          model.dstate=[ci_cnt]
          model.ipar=[nz;ev;length(varnam);str2code(varnam)]
          model.sim=list('sci_buf',4)
        else //z_buf
          model.dstate=[ci_cnt;zeros(lz,1)]
          model.ipar=[nz;ev]
          model.sim=list('z_buf41',4)
        end
        graphics.exprs=exprs
        x.graphics=graphics;
        x.model=model;
        break
      end
   end

case 'define' then
  nu=-1
  nz=128
  ev=0
  varnam=''
  ci_cnt=0
  herit=0

  model=scicos_model()
  model.sim=list('z_buf41',4)
  model.in=[nu]
  model.out=[]
  model.evtin=[1]
  model.evtout=[]
  model.dstate=[ci_cnt;zeros(nz,1)]
  model.rpar=[]
  //model.ipar=[nz;ev;length(varnam);str2code(varnam)]
  model.ipar=[nz;ev]
  model.blocktype='d'
  model.firing=[]
  model.dep_ut=[%f %f]

  gr_i=['txt=[''BUFFER''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  exprs=[string(nu),string(nz),string(ev),string(varnam),string(ci_cnt),string(herit)]
  x=standard_define([2 2],model,exprs,gr_i)
  x.graphics.id="Out"
end
endfunction

function ok=check_varname(str)
  ok=%t;
  //please update
  protect_var=['t';'%cpr';'%tcur';'tf';'tolerances';
               'needstart';'curwin'];

  if find(str==protect_var)<>[] then
   ok=%f
   return
  end
  ierr=execstr('clear('''+str+''')','errcatch');
  if ierr<>0 then ok=%f, return, end;
  ierr=execstr(str+'=='+str,'errcatch');
  if ierr<>4 then ok=%f, return, end;
  ierr=execstr(str+'=0','errcatch');
  if ierr<>0 then ok=%f, return, end;
endfunction

function ok=check_varbuf(str)
 GUI=['ZBUF_c';
      'ZBUFT_c'];

 //indice dans exprs de varnam
 exprs_ind=[4;  //ZBUF_c
            2]; //ZBUFT_c

 ok=%t
 i_ind=[]
 j_ind=[]

 for i=1:size(scs_m.objs)
    if typeof(scs_m.objs(i))=='Block' then
      for j=1:size(GUI,1)
        if scs_m.objs(i).gui==GUI(j) then
          i_ind=[i_ind;i];
          j_ind=[j_ind;j];
          break;
        end
      end
    end
 end

 if i_ind<>[] then
   if size(i_ind,1)<>1 then
     for i=1:size(i_ind,1)
       if ~isequal(scs_m.objs(i_ind(i)),arg1) then
          if scs_m.objs(i_ind(i)).graphics.exprs(exprs_ind(j_ind(i)))==str then
            ok=%f;
            return
          end
       end
     end
   end
 end
endfunction