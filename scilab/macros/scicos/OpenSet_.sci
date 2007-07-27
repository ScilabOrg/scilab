function OpenSet_()
//** INRIA 
//** 24 May 2006:
//** 10 Jul 2006:
//** 12 Jul 2006:
//** 25 Jully 2007 : 
//** Comments by Simone Mannori

disablemenus() ; //** disable the "interrupts" from dynamic menu :)
  
  %xc = %pt(1); %yc = %pt(2); //** last mouse position
 

  if windows(find(%win==windows(:,2)),1)==100000 then
    //** ------------------ Navigator -------------------------------
    //click in navigator
    [%Path,%kk,ok] = whereintree(%Tree,%xc,%yc)
    if ok & %kk<>[] then %Path($)=null(); %Path($)=null(); end
    if ~ok then %kk=[], end
  else
    //** ---- Is not a navigator -> look for the object -------------
    %kk = getobj(scs_m,[%xc;%yc]) ; //** acquire the index in the current diagram 
    %Path = list('objs',%kk)      ; //** create the path to the object   
  end

  //** '%kk' is the object index

  //**---------------------------------------------------------------------------
  if %kk<>[] then //** if the double click is not in the void -------------------
    //**
    Select_back = Select; 
    selecthilite(Select_back, "off") ; //  unHilite previous objects
    Select = [%kk %win];               //** select the double clicked block 
    selecthilite(Select, "on") ;       //  Hilite the actual selected object
    //**
    super_path = [super_path, %kk] ; 
    [o, modified, newparametersb, needcompileb, editedb] = clickin( scs_m(%Path) );
    //** BEWARE : "clickin can modify the "Cmenu" to force the creation of a Link  
    
    //** this POC potentially dangerous !!!
    if Cmenu=="Link" then
      %pt = [%xc, %yc]   ;
      super_path($) = [] ;
      enablemenus()      ;
        return ; //** ---> EXIT point
    end

    //** exit from superblock if parent window is destroyed
    //**--------------------------------------------------------------------------
    // in case previous window has been destroyed
    if ~or(curwin==winsid()) then //** if there are NOT windows inside the list
      gh_curwin = scf(curwin);
      gh_current_window = gh_curwin ; //** ... non ne sono proprio sicuro ....
                                      //** ... pero' sembra che funzioni .....
      clf(gh_curwin) ;
      gh_curwin.pixmap = "on"

      if ~set_cmap(scs_m.props.options('Cmap')) then // add colors if required
        scs_m.props.options('3D')(1)=%f //disable 3D block shape
      end

      set_background(gh_curwin)   ;
      pwindow_set_size(gh_curwin) ;
      window_set_size(gh_curwin)  ;
      drawobjs(scs_m, gh_curwin)  ; //** redraw all the objects !
      menu_stuff() ;

      if ~super_block then
	           delmenu(curwin,'stop')
	           addmenu(curwin,'stop',list(1,'haltscicos'))
	           unsetmenu(curwin,'stop')
      else
	unsetmenu(curwin,'Simulate')
      end
    end  // end of redrawing deleted parent
    //-----------------------------------------------------------------------

    
    //** Distruction (closing) of a Navigator window 
    if needcompileb==4 then
      %kw = find(windows(:,1)==100000)
      if %kw<>[] then
	xdel(windows(%kw,2)) ; //** DANGER : DANGER : DANGER
	%Tree = list()
      end
    end

    edited = edited | editedb ;
    super_path($-size(%kk,2)+1:$) = [] ;

    if editedb then
      scs_m_save = scs_m       ; //** save the old diagram 
      nc_save    = needcompile ; //** and its state 
      
      if ~pal_mode then
	       needcompile = max(needcompile, needcompileb)
      end
      
      scs_m = update_redraw_obj(scs_m, %Path,o) ; //** DANGER DANGER DANGER
    
    end

    // note if block parameters have been modified
    if modified&~pal_mode  then
      newparameters = mark_newpars(%kk,newparametersb,newparameters) ; //** DANGER
    end

  end //**.. the double click was not in the void 

  Cmenu = []; %pt = [] ;
  
  enablemenus()      ; 
  
endfunction
