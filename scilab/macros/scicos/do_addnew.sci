// Copyright INRIA

//** 22 May 2006: Looking for a bug inside the new graphics handling 

// add a new block (defined by its GUI function to a palette

//** "scs_m" is the diagram datastructure

//** "fct" is a flag control variable used by the calling function Addnewblock
//**       it is used to signal modification of the datastructure

function [scs_m, fct] = do_addnew(scs_m)

//**---------- This part handle the file related functions and error handling --------------

  fct = [] ;
  [ok,name] = getvalue( 'Get block GUI function name', ['Name'],list('str',1),emptystr() )

  if ~ok then return, end ; //** --> Exit point

  name = stripblanks(name);

  if name==emptystr() then message('No block name specified');return,end ; //** --> Exit point

  to_get=%f

  if exists(name)==0 then
    to_get=%t
  else
    execstr('tp=type('+name+')')
    to_get=tp<>11&tp<>13
  end

  if to_get then // try to get it
    path=name+'.sci'
    path=getfile("*.sci",path,title="File containing function: "+name)

    if length(path)<=0 then return,end
    [u,err]=file('open',path,'old','formatted')

    if err<>0 then
      message(path+' file, Not found')
      return
    end

    if execstr('getf(u)','errcatch')<>0 then
      file('close',u)
      message([name+' erroneous function:';lasterror()])
      return
    end

    file('close',u)

    if ~exists(name) then message(name+' is not defined in this file'),return,end

    fct=path

  else

    fct=emptystr()

  end

  //define the block
  ierror = execstr('blk='+name+'(''define'')','errcatch')
  if ierror <>0 & ierror <>4 then
    message(['Error in GUI function';lasterror()] )
    fct=[]
    return
  end

  if ierror == 4 then
    irr=message(['Error in GUI function--The error was:';
		lasterror();'It could be an old GUI';
	     'Should I try to translate (no guarantee)?'],['yes','no'])
    if irr==2 then fct=[];return

    else
      funcprot_val=funcprot()
      funcprot(0)
      standard_define=standard_define_old;
      funcprot(funcprot_val)
      ierror=execstr('blk='+name+'(''define'')','errcatch')

      if ierror <>0 then
	         message("Translation did not work, sorry" )
	         fct=[]
	         return
      end

      do_version=do_version;//load do_version and its subfunctions

      ierror=execstr('blk=up_to_date(blk)','errcatch');

      if ierror <>0 then
	          message("Translation did not work, sorry" )
	          fct=[]
	          return
      end

    end

  end

  //**------ Al@n's update ---------/////////////
  if blk.model.sim(1)=='super'|...
      blk.model.sim(1)=='csuper' then

    scs_m_super = blk.model.rpar;

    //check version
    current_version = get_scicos_version()
    if type(scs_m_super)==17 then
      if find(getfield(1,scs_m_super)=='version')<>[] then
        if scs_m_super.version<>'' then
          scicos_ver=scs_m_super.version
        else
          if find(getfield(1,blk.model)=='equations')<>[] then
            scicos_ver = "scicos2.7.3" //for compatibility
          else
            scicos_ver = "scicos2.7" //for compatibility
          end
          message(["Warning : you want to import an old compiled super block.";
                   "I will try to translate this block by generating ";
                   "a new interfacing function in TMPDIR"])
        end
      else
        if find(getfield(1,blk.model)=='equations')<>[] then
          scicos_ver = "scicos2.7.3" //for compatibility
        else
          scicos_ver = "scicos2.7" //for compatibility
        end
        message(["Warning : you want to import an old compiled super block.";
                 "I will try to translate this block by generating ";
                 "a new interfacing function in TMPDIR"])
      end
    else
      message("Can''t import block in scicos, sorry" )
      fct=[]
      return
    end

    //do version
    if scicos_ver<>current_version then
      scs_m_super=do_version(scs_m_super,scicos_ver)
      blk.model.rpar = scs_m_super;

      //generate a new interfacing function in TMPDIR
      if blk.model.sim(1)=='super' then
        save_super(scs_m_super,TMPDIR)
      elseif blk.model.sim(1)=='csuper' then
        save_csuper(scs_m_super,TMPDIR)
      end
      message(name+".sci generated in "+TMPDIR+"." )

      //load new interfacing function
      [u,err]=file('open',TMPDIR+'/'+name+'.sci','old','formatted')
      if err<>0 then
       message(TMPDIR+'/'+name+'.sci'+' file, Not found')
       return
      end
      if execstr('getf(u)','errcatch')<>0 then
       file('close',u)
       message([name+' erroneous function:';lasterror()])
       return
      end
      file('close',u)
      fct=TMPDIR+'/'+name+'.sci'

      //define the block
      ierror = execstr('blk='+name+'(''define'')','errcatch')
      if ierror <>0 & ierror <>4 then
        message(['Error in GUI function';lasterror()] )
        fct=[]
        return
      end
    end
  end
  //**------------------------------------/////////////

//**--------------------------------------------------------------------------------------
//** The requested block is valid, then it is accepted
//** ---------------------- The graphics interaction begin here --------------------------

//** This code basically works ( :-) ) : it is the "Replot" that need attention :)

  xinfo('Choose block position in the window')

  blk.graphics.sz = 20*blk.graphics.sz;
  %xc = 100; %yc = 100; //** default start position
   xy =[%xc,%yc] ;
  blk.graphics.orig = xy

  gh_blk=drawobj(blk); //** draw the block (in the buffer) - using the corresponding Interface Function
                       //** Scilab Language - of the specific block (blk) and get back the graphic handler
		       //** to handle the block as a single entity

//**-----------------------------------------------------------------
//** ---> main loop that move the empty box until you click
  rep(3)=-1 ;
  while rep(3)==-1 , //move loop

    // get new position
    rep = xgetmouse(0,[%t,%t])

    //** Protection from window closing
    if rep(3)==-100 then //active window has been closed
      //** driver(dr); //** obsolete removed
      [%win,Cmenu] = resume(curwin,'Quit')
    end

    xm=rep(1) ; ym = rep(2) ;
    dx = xm - %xc ; dy = ym - %yc ;
    drawlater();
    move (gh_blk , [dx dy]);
    draw(gh_blk.parent);
    show_pixmap();

    %xc = xm ;%yc = ym ; //** position update

  end //** ---> of the while loop
//**----------------------------------------------------------------------
  //** window closing protection
  if xget('window') <> curwin then
    //active window has been closed
    [%win,Cmenu]=resume(curwin,'Quit')
  end

  xinfo(' ')

  xy = [%xc,%yc];
  blk.graphics.orig = xy ; //** update object position in the data strucure

  scs_m.objs($+1) = blk ; //** add the object to the data structure

  // disp("After Object Positioning"); pause

endfunction

//**---------------------------------------------------------------------------------------------------

function objsi =up_to_date(o)
  if size(o(2)) > 8 then
    if type(o(2)(9))==15 then 
      gr_io=o(2)(9)(1);
      if o(2)(9)(2)<>[] then
	back_col=o(2)(9)(2);,
      end
    else
      gr_io=o(2)(9);
      back_col=8
    end
    gr_i=convert_gri(o(5),gr_io);
    if gr_i==[] then gr_i=gr_io;, end
  elseif size(o(2)) < 9 then
    gr_i=[];
    back_col=8
  end
  gr_i=list(gr_i,back_col)

  mdl=o(3);
  if size(o(3))<=12 then 
    mdl(13)=''; mdl(14)=[] ; mdl(15)='';
  elseif size(o(3))<=13 then 
    mdl(14)=[] ; mdl(15)='';
  elseif size(o(3))<=14 then 
    mdl(15)='';
  end

  if mdl(1)(1)=='super'|mdl(1)(1)=='csuper' then
    if type(mdl(8))==15 then
      mdl(8)=do_version27(mdl(8))
    end
  end

  graphics=scicos_graphics(orig=o(2)(1),sz=o(2)(2),flip=o(2)(3),..
			   exprs=o(2)(4),pin=o(2)(5),pout=o(2)(6),..
			   pein=o(2)(7),peout=o(2)(8),gr_i=gr_i,..
			   id=mdl(15))

  model=scicos_model(sim=mdl(1),in=mdl(2),out=mdl(3),evtin=mdl(4),..
		     evtout=mdl(5),state=mdl(6),dstate=mdl(7),..
		     rpar=mdl(8),ipar=mdl(9),blocktype=mdl(10),..
		     firing=mdl(11),dep_ut=mdl(12),label=mdl(13))
  
  
  objsi=scicos_block(graphics=graphics,model=model,gui=o(5),..
		     doc=mdl(14))
  if objsi.gui=='ESELECT_f' then objsi.model.sim(2)=-2,end

endfunction

//** --------------------------------------------------------------------------------------------

function o=standard_define_old(sz,model,label,gr_i)
//initialize graphic part of the block data structure
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<4 then gr_i=[],end
[nin,nout,ncin,ncout]=model(2:5)
nin=size(nin,1);if nin>0 then pin(nin,1)=0,else pin=[],end
nout=size(nout,1);if nout>0 then pout(nout,1)=0,else pout=[],end
ncin=size(ncin,1);if ncin>0 then pcin(ncin,1)=0,else pcin=[],end
ncout=size(ncout,1);if ncout>0 then pcout(ncout,1)=0,else pcout=[],end
graphics=list([0,0],sz,%t,label,pin,pout,pcin,pcout,gr_i)
if model(1)(1)=='super' then
   o=list('Block',graphics,model,' ','SUPER_f')
else
  [ln,mc]=where()
  o=list('Block',graphics,model,' ',mc(2))
end
endfunction

