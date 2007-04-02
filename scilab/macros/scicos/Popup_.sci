function Popup_()
//** INRIA
//**
//** 
//** This function uses the "%scicos_lhb_list(state_var) data strucuture
//** 
//** state_var = 1 : right click over a valid object inside the CURRENT Scicos Window
//** 
//** state_var = 2 : right click in the void of the CURRENT Scicos Window
//** 
//** state_var = 3 : right click over a valid object inside a PALETTE 

  kc = find( %win==windows(:,2) );

  //**---------------------------------------------------------------------
  if kc==[] then
  //** ------------- It's NOT a Scicos window -------------------
    message("This window is not an active scicos window")
    Cmenu = []; %pt=[]; %ppt=[]; Select=[]; return ; //** ---> Exit point 

  //**--------------- Palette -----------------------------------
  elseif windows(kc,1)<0 then // click inside a palette window

    gh_curwin = scf(%win) ;
    o_size = size(gh_curwin.children.children)

    kpal = -windows(kc,1)    ;
    palette = palettes(kpal) ;
    k = getobj(palette,%pt)  ;

    if k<>[] then 
      gh_k   = get_gri(k,o_size(1))
      gh_blk = gh_curwin.children.children(gh_k);
      state_var = 3 ; //** magic number by Ramine:
                      //** vaild object inside a palette 
		      //** yoy cannot modify its proprieties !
      
      Select = [k,%win];
      selecthilite(Select,'on') ; // immediate select operation 

    else
      //** in the void of a palette 
      Cmenu==[]; %pt=[]; %ppt=[]; Select=[]; return ; //** ---> Exit point 

    end

  //**-------- pupup in the Current Scicos window (main or superblock) -----------
  elseif %win==curwin then //click inside the current window 

    gh_curwin = scf(%win) ;
    o_size = size(gh_curwin.children.children)
    k = getobj(scs_m,%pt)
    
    if k<>[] then 
      //** popup over a valid object 
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
    
  //**-------- popup NOT the Current Scicos window (other scicos win) ----------
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
      Cmenu==[]; %pt=[]; %ppt=[]; Select=[]; return ; //** ---> Exit point 
    end
    
  else
  //**---- ... in any other case -------------------------------  
    message("This window is not an active scicos window")
    Cmenu=[]; %pt=[]; %ppt=[]; Select=[]; return ; //** ---> Exit point 
  
  
  end //** end of the main if() switch case structure
  //**--------------------------------------------------------------------------
  
  
  //** The main external function call 
  //** disp("Popup state_var="); disp(state_var); //** debug only :)
  
  //** the key function call that (see "scicos.cos") call 
  //** "mpopup => tk_mpopupX" Penguin
  //** "mpopup => tk_mpopup " WindowZed
  Cmenu = mpopup( %scicos_lhb_list(state_var) ) ; //**

  if Cmenu==[] then
       %pt  = [];
       %ppt = [];
       selecthilite(Select, 'off') ; //** update the screen 
       Select = [];
  end

endfunction




