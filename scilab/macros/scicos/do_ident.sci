function scs_m = do_ident(scs_m)
// Copyright INRIA
//** 06 Set 2006 
  
  if Select==[] then //** if no object are selected, 
    xc = %pt(1);     //** take the last "cosclic" mouse position  and 
    yc = %pt(2)  ;
    k = getobj(scs_m,[xc;yc]) ; //** fook for an object 
    if k==[] then return,end  ; //** if no object --> EXIT 
  else
    k = Select(:,1)'; %pt=[]  ; //** ... otherwise take the object 
  end  
  
  //** Filter out the "mutiple object selected" case 
  if size(k,'*')>1 | %win<>Select(1,2) then
    message("Only one block can be selected in current window for this operation.")
    Cmenu=[]; %pt=[]; return
  end
  
  numero_objet = k     ; //** object ID  
  scs_m_save   = scs_m ; //** save the diagram (for "Cancel" operation) 

  objet = scs_m.objs(numero_objet) ; //** isolate the object data structure 
  type_objet = typeof(objet)       ; 

  //** select the possible cases 
  
  if type_objet == 'Block' then
    //** -------- BLOCK --------------
    identification = objet.graphics.id
    if identification == [] then
      identification = emptystr() ; 
    end
    //** Use a dialog box to acquire/modify the id string
    texte_1 = 'Set Block identification' ;
    texte_2 = 'ID'                       ;
    [ok, identification] = getvalue(texte_1, texte_2, list('str', 1), identification) ; 
    
    if ok then
      //** drawobj(objet) ;/** old graphics xor mode 
      objet.graphics.id = stripblanks(identification); //** update the identification structure 
      //** drawobj(objet)
      scs_m.objs(numero_objet) = objet ; //** update the object data structure
    end
    //**----------------------------
  elseif type_objet == 'Link' then
    //** ----- LINK -----------
    identification = objet.id ; 
    if identification == [] then
      identification = emptystr() ; 
    end
    //** Use a dialog box to acquire/modify the id string
    texte_1 = 'Set link Identification' ;
    texte_2 = 'ID'                      ;
    [ok, identification] = getvalue(texte_1, texte_2, list('str', 1),identification) ;
    //
    if ok then
      identification = stripblanks(identification)  ;
      c_links = connected_links(scs_m, numero_objet) ;
      //- set identification to all connected links
      for numero = c_links ; 
	//** drawobj(scs_m.objs(numero));//** old XOR mode 
	scs_m.objs(numero).id = identification
	//** drawobj(scs_m.objs(numero)); //** 
      end
      //
    end				
  else
  //** It is NOT a Block AND it is NOT a Link: for any other object type 
    x_message('It is impossible to set ID for this type of object')
  end
  //
  
  if ok then [scs_m_save,enable_undo,edited] = resume(scs_m_save,%t,%t),end

endfunction
