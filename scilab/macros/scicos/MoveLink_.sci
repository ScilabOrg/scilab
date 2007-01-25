function MoveLink_()
//** 20 Jan 2006
//** 10 Jan 2007 
//** Comments and mods by Simone Mannori 

//** This function is activated by a left button press 
  
  //** the press is inside a navigator window
  if windows(find(%win==windows(:,2)),1)==100000 then //** Navigator window:
    Cmenu=[] ; %pt=[] ; //** clear the variable 
    return; //** and exit 
    
  elseif %win<>curwin then //** the press is not in the current window 
    
    kc = find(%win==windows(:,2)); 
    
    if kc==[] then //** the press is not inside an scicos actiview window 
      message('This window is not an active scicos window')
      Cmenu=[]; %pt=[]; return
    elseif windows(kc,1)<0 then //** the press is inside a palette 
      kpal = -windows(kc,1)    ; 
      palette = palettes(kpal) ;
      %kk = getobj(palette,%pt) ; //** get the obj inside the palette 
    elseif slevel>1 then //** the press is over a block inside a superblock window
      execstr('%kk=getobj(scs_m_'+string(windows(kc,1))+',%pt)')
    end
    
    if %kk<>[] then //** press over a valid block 
        Cmenu="Duplicate"
        Select=[%kk,%win]
    else           //** press in the void   
        Cmenu="SelectRegion"
        Select=[];
    end
    
  else //** ... the press is in the current window 
    //** look for an object 
    %kk = getobj(scs_m,%pt)
    //** if an object id found 
    if %kk<>[] then

       //** if one or more object are selected CALL direct Move 
       if Select<>[] then  
         Cmenu="Move" ; //** execute Move ; NB: the %pt information is preserved  
       else
         //** executed ONLY if NO object are selected 
         Cmenu = check_edge(scs_m.objs(%kk),'Move',%pt)
         //** inside "check_edge"  - as default - the Cmenu variable is pre-loaded 
         //** (look in the calling arguments) with 'Move' action command. 
         //** If check_edge(...) does NOT find a input port, Cmenu remain 'Move' then issue a Move_.sci 
         //** ... else
         //** Cmenu='Link' then issue a Link_.sci
         Select = [%kk, %win]; //** select the pressed object
       end
        
     else //** if the press is in the void of the current window 
       Cmenu = "SelectRegion" ; //** "SelectRegion" will be called later 
       %ppt=[]; Select=[] ;  //** NB: the %pt information is preserved for SelectRegion operation 
    end
  
  end

endfunction


