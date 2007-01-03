function Popup_()
//** INRIA
//**
  //** at this point I need to build the [scs_m] <-> [gh_window] datastructure 
  //** I need an equivalent index for the graphics

  // o_size = size (gh_curwin.children.children ) ; //** the size:number of all the object 

  //** "k" is the object index in the data structure "scs_m"
  //** compute the equivalent "gh_k" for the graphics datastructure 
  // gh_k = o_size(1) - k + 1 ; //** semi empirical equation :) 
  // gh_blk = gh_curwin.children.children(gh_k);

  kc = find( %win==windows(:,2) );

  //**------------------------------------------------------------------------------
  if kc==[] then

  //** ------------- It's NOT a Scicos window -------------------
    message('This window is not an active scicos window')
    Cmenu = []; %pt=[]; %ppt=[]; Select=[]; return ; //** ---> Exit point 

  //**--------------- Palette -----------------------------------
  elseif windows(kc,1)<0 then // click inside a palette window

    gh_curwin = scf(%win) ;
    o_size = size(gh_curwin.children.children)

    kpal = -windows(kc,1)    ;
    palette = palettes(kpal) ;
    k = getobj(palette,%pt)  ;

    if k<>[] then 
      //gh_k = o_size(1) - k + 1 ;
      gh_k=get_gri(k,o_size(1))
      gh_blk = gh_curwin.children.children(gh_k);

      //** twinkle(gh_blk,2);
      state_var = 3 ; //** magic number by Ramine
      Select = [k,%win];
	  selecthilite(Select, 'on') ;       // update the image

    else
      Cmenu==[];%pt=[];%ppt=[];Select=[];return

    end

  //**--------------- Current Scicos window --------------------
  elseif %win==curwin then //click inside the current window 

    gh_curwin = scf(%win) ;
    o_size = size(gh_curwin.children.children)
    k = getobj(scs_m,%pt)
    if k<>[] then
      state_var = 1; //** Magic number by Ramine
      //gh_k = o_size(1) - k + 1 ;
      gh_k=get_gri(k,o_size(1))
      gh_blk = gh_curwin.children.children(gh_k);
      //** twinkle(gh_blk,2);
      Select = [k,%win]
      selecthilite(Select, 'on') ;       // update the image
    else
      state_var = 2; //** magic number by Ramine
      %ppt = %pt    // for pasting
    end
  //**--------------- Superblock Scicos window -----------------
  elseif slevel>1 then
    execstr('k=getobj(scs_m_'+string(windows(kc,1))+',%pt)')
    gh_curwin = scf(%win) ;
    o_size = size(gh_curwin.children.children)
    if k<>[] then
      //gh_k = o_size(1) - k + 1 ;
      gh_k=get_gri(k,o_size(1))
      gh_blk = gh_curwin.children.children(gh_k);
      //** twinkle(gh_blk,2);
      Select=[k,%win];
      selecthilite(Select, 'on') ;       // update the image
      state_var = 3 ; //** Magic number by Ramine
    else
      Cmenu==[];%pt=[];%ppt=[];Select=[];return
    end
  else
    message('This window is not an active scicos window')
    Cmenu=[];%pt=[];%ppt=[];Select=[];return
  end //** end of the main if() switch case structure

  Cmenu = mpopup( %scicos_lhb_list(state_var) ) ;

  if Cmenu==[] then
       %pt  = [];
       %ppt = [];
       selecthilite(Select, 'off') ;       // update the image
       Select = [];
  end

endfunction
