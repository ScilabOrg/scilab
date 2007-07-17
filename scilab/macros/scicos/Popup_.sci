function Popup_()
//** INRIA
//**
//** 
//** This function uses the "%scicos_lhb_list(state_var) data strucuture
//** defined inside [auto/scicos.sci]
//** 

state_var = 0 ; //** init

//** state_var = 1 : right click over a valid object inside the CURRENT Scicos Window
//** 
//** state_var = 2 : right click in the void of the CURRENT Scicos Window
//** 
//** state_var = 3 : right click over a valid object inside a PALETTE or NOT a CURRENT Scicos Window


state_pal = 0 ;  

//** state_var = 0 : no Select/Unselect operation on Palette
//** state_var = 1 : Select/Unselect operation on Palette: the correct state of the propieties 
//**                 of the figure should be restored ad the end 


gh_winback = gcf() ; //** save the active window
    
  kc = find( %win==windows(:,2) );

  //**---------------------------------------------------------------------
  if kc==[] then
  //** ------------- It's NOT a Scicos window -------------------
    message("This window is not an active scicos window")
    Cmenu = []; %pt=[]; %ppt=[]; Select=[] ;
    scf(gh_winback); //** restore the active window   
    return ; //** ---> Exit point 

  //**--------------- Palette -----------------------------------
  elseif windows(kc,1)<0 then //** RIGTH click inside a palette window

    gh_curwin = scf(%win) ;
    
    kpal = -windows(kc,1)    ;
    palette = palettes(kpal) ;
    k = getobj(palette,%pt)  ;

    if k<>[] then
    
      state_var = 3 ; //** magic number by Ramine:
                      //** valid object inside a palette 
		      //** you cannot modify its proprieties !
      
      state_pal = 1 ; //** mark the palette alteration  
		      
      o_size = size(gh_curwin.children.children);  
      gh_k   = get_gri(k,o_size(1)) ;
      gh_blk = gh_curwin.children.children(gh_k);
      
      Select = [k,%win];
      selecthilite(Select,"on") ; // immediate select operation 
      
    else
      //** in the void of a palette 
      Cmenu==[]; %pt=[]; %ppt=[]; Select=[];
      scf(gh_winback); //** restore the active window   
      return ; //** ---> Exit point 
    end
  
  //**--- pupup in the CURRENT Scicos window : Main Scicos Window (not inside a superblock) ----------
  elseif %win==curwin then //click inside the current window 

    gh_curwin = scf(%win) ;
    o_size = size(gh_curwin.children.children)
    k = getobj(scs_m,%pt)
    
    if k<>[] then 
      //** popup over a valid object in the current Scicos window 
      state_var = 1; //** Magic number by Ramine
                     //** You can modify all the proprieties of an object 
      gh_k = get_gri(k,o_size(1))
      gh_blk = gh_curwin.children.children(gh_k);
      
      Select = [k,%win]
      selecthilite(Select, 'on') ; //** immediate screen update 
    else
      //** popup in the void 
      state_var = 2; //** magic number by Ramine
      %ppt = %pt     // for pasting
    end
    
  //**--- pupup in a SuperBlock Scicos Window that is NOT the current window ----------
  elseif slevel>1 then
    execstr('k = getobj(scs_m_'+string(windows(kc,1))+',%pt)')
    gh_curwin = scf(%win) ;
    o_size = size(gh_curwin.children.children)
    
    if k<>[] then
      gh_k   = get_gri(k,o_size(1))
      gh_blk = gh_curwin.children.children(gh_k);
      Select = [k,%win];
      selecthilite(Select, 'on') ; // update the image
      state_var = 3 ; //** Magic number by Ramine
                      //** read only operation 
    else
      //** in the void 
      Cmenu==[]; %pt=[]; %ppt=[]; Select=[];
      scf(gh_winback); //** restore the active window   
      return ; //** ---> Exit point 
    end
    
  else
  //**---- ... in any other case -------------------------------  
    message("This window is not an active scicos window")
    Cmenu=[]; %pt=[]; %ppt=[]; Select=[];
    scf(gh_winback); //** restore the active window   
    return ; //** ---> Exit point 
  
  
  end //** end of the main if() switch case structure
  //**--------------------------------------------------------------------------
  
  
  //** The main external function call 
  //** disp("Popup state_var="); disp(state_var); //** debug only :)
  
  //** the key function call that (see "scicos.cos") call 
  //** "mpopup => tk_mpopupX" Penguin   (Unix/Linux)
  //** "mpopup => tk_mpopup " WindowZed (Windows)
  
  Cmenu = mpopup( %scicos_lhb_list(state_var) ) ; //**

  if Cmenu==[] then
       %pt  = [];
       %ppt = [];
       selecthilite(Select, "off") ; //** update the screen 
       Select = [];
  end
  
  //** restore the state for palette window 

  if state_pal==1 then //** inside a palette window
     //** force the proprieties of the palette windows:
     //** the user should be capable to change the size  
     gh_curwin.auto_resize = "on"       ; //** 
     gh_curwin.pixmap      = "off"      ;
     gh_curwin.immediate_drawing = "on" ;
  end 
  
scf(gh_winback); //** restore the active window     
  
endfunction




