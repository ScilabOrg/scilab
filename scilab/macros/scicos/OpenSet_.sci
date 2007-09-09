function OpenSet_()
//** INRIA 
//** 24 May 2006:
//** 10 Jul 2006:
//** 12 Jul 2006:
//** 25 Jully 2007 : 
//** Comments by Simone Mannori

  global inactive_windows

  if or(curwin==winsid()) then gh_current_window=gcf(curwin);end

  if ~%diagram_open then
    %kk=Select(1)
    if size(scs_m.objs)<%kk then
      ierr=1
    else
      ierr=execstr('xxx=scs_m.objs(%kk).model.sim','errcatch','n')
    end
    if ierr==0 then
      if xxx<>'super' then ierr=1;end
    end
    if ierr<>0 then
      message(['This super block is not active anymore or';
	       'the browser is not up-to-date.'])
      %scicos_navig=[]  // stop navigation
      return
    end
    
    inactive_windows(1)($+1)=super_path;inactive_windows(2)($+1)=curwin

    super_path = [super_path, %kk] ; 
    
    [o, modified, newparametersb, needcompileb, editedb] =..
	clickin( scs_m.objs(%kk));

    indx=find(curwin==inactive_windows(2))
    if indx<>[] then
        inactive_windows(1)(indx)=null();inactive_windows(2)(indx)=[]
    end
    
    edited = edited | editedb ;
    super_path($-size(%kk,2)+1:$) = [] ;
    
    if editedb then
      scs_m_save = scs_m       ; //** save the old diagram 
      nc_save    = needcompile ; //** and its state 
      needcompile = max(needcompile, needcompileb)
      %Path = list('objs',%kk)
      if or(curwin==winsid()) then gh_current_window=gcf(curwin);end
      scs_m = update_redraw_obj(scs_m, %Path,o) ;//scs_m.objs(%kk)=o
    end
    
    if modified then
      newparameters = mark_newpars(%kk,newparametersb,newparameters) ; 
    end
    return
  end
   
//////////////////////////////////////////////////////////////////////
  
  %xc = %pt(1); %yc = %pt(2); //** last mouse position
  
  %kk = getobj(scs_m,[%xc;%yc]) ; //** acquire the index in the current diagram 
  %Path = list('objs',%kk)      ; //** create the path to the object   
  

  //** '%kk' is the object index

  //**-----------------------------------------------------------------
  if %kk<>[] then //** if the double click is not in the void ---------
    Select_back = Select; 
    selecthilite(Select_back, "off") ; //  unHilite previous objects
    Select = [%kk %win];               //** select the double clicked block 
    selecthilite(Select, "on") ;       
				       
    inactive_windows(1)($+1)=super_path;inactive_windows(2)($+1)=curwin
		       
    super_path = [super_path, %kk] ; 
    [o, modified, newparametersb, needcompileb, editedb] = clickin( ...
	scs_m(%Path) );

    indx=find(curwin==inactive_windows(2))
    if indx <> [] then
      inactive_windows(1)(indx)=null();inactive_windows(2)(indx)=[]
    end
    
    //** BEWARE : "clickin can modify the "Cmenu" 
    //to force the creation of a Link  
    
    //** this POC potentially dangerous !!!
    if Cmenu=="Link" then
      %pt = [%xc, %yc]   ;
      super_path($) = [] ;
      return ; //** ---> EXIT point
    end
    
   
    edited = edited | editedb ;
    super_path($-size(%kk,2)+1:$) = [] ;
    
    if editedb then
      scs_m_save = scs_m       ; //** save the old diagram 
      nc_save    = needcompile ; //** and its state 
      
      if ~pal_mode then
	needcompile = max(needcompile, needcompileb)
      end
      if or(curwin==winsid()) then gh_current_window=gcf(curwin);end
      scs_m = update_redraw_obj(scs_m, %Path,o) ; //** DANGER DANGER DANGER
      
    end
    
    // note if block parameters have been modified
    if modified  then
      newparameters = mark_newpars(%kk,newparametersb,newparameters) ; //** DANGER
    end
    
  end //**.. the double click was not in the void 
  
  Cmenu = []; %pt = [] ;
  
//  enablemenus()      ; 
  
endfunction


