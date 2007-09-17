function [btn, %pt, win, Cmenu ] = cosclick(flag)
//** INRIA
//** Comments by Simone Mannori 
//** btn : button / event id (numeric code)
//** %pt : mouse position[x,y] of the event
//** win : Scilab/Scicos windows where the event occour
//** 
//** 12 July 2006 : Revision
//**

  [lhs,rhs] = argn(0) ;
  
  Cmenu=[]; %pt=[]; btn = 0 ; //** Clear variables
  
  //** --> This control is really indispensable because the user can                       //
  //**     - click over a Scicos window, then (very quickly)                               //
  //**     - Close the window clicking over the [X] button                                 //
  //** if the current Scicos windows is NOT present in the list of Scilab window           //
  if ~or( winsid()==curwin ) then //**                                                     //
    //** disp("...cosclic.sci: fast Select then CloseWindow...|:"); pause; //** debug only //
    win = curwin   ;
    Cmenu = "Quit" ; 
    return         ; //** EXIT Point //
  end   
  //**----------------------------------------------------------------------------//
  
  //** ------------> This check will disappear ? -------------------------------- // 
  if rhs==1 then
    [btn, xc, yc, win, str] = xclick(flag) //** not used now (was used in the past) 
  else
    [btn, xc ,yc ,win ,str ] = xclick()    //** <- This is used in the main scicos_new() loop:
  end                                      //**    CLEAR ANY PREVIOUS EVENT in the queue
  //**--------------------------------------------------------------------------- //
  
  %pt = [xc,yc] ; //** acquire the position  
  
  //**--------------------------------------------------------------------------
  //** cosclic() filter and command association 
  
  //**--------------------------------------------------------------------------
 global inactive_windows  
 
  if btn==-100 then //** window closing check 
  //**------------------------------------------------------------
  //** The window has been closed 
    
    if win==curwin then  //** in the current window ? 
      Cmenu = "Quit" ;     
    else                 //** not the current window 
      Cmenu = []    ;
      %pt   = []    ;
    end
    
    return  //** --> EXIT  
  end

  //**-----------------------------------------------------------
  if (btn==30) then //** This code is produced ONLY on Windows and is used
                        //** to signal the switch of the focus to a new, not yet active,
			//** Scilab window. This code is produced ONLY ONE time, then
			//** the xclick return back to the usual behavior.
     btn=3  // assimilated with single click, better than nothing!
  end
  if (win==-1)& (btn==-2)&part(str,1:7)=='execstr' then
    from=max(strindex(str,'_'))+1;
    to=max(strindex(str,'('))-1
    win=evstr(part(str,from:to))
  end
  // If the event is a TCL event then win=[]
  if win<>[] & find(win==inactive_windows(2))<>[] then
    global Scicos_commands
    pathh=inactive_windows(1)(find(win==inactive_windows(2)))
    cmd='Cmenu=''SelectLink'''
    if btn==-2 then
         cmd='Cmenu='+part(str,9:length(str)-1)+';execstr(''Cmenu=''+Cmenu)'
    end

    Scicos_commands=['%diagram_path_objective='+sci2exp(pathh)+';%scicos_navig=1';
		     cmd+';%pt='+sci2exp(%pt)+';xselect();%scicos_navig=[]';
		    ]

    return
  
  //** -----------------------------------------------------------
  elseif (btn==3) then //** Single click : Left Mouse Button : no window check         
    Cmenu = "SelectLink" ; 
    
  //** -----------------------------------------------------------
  elseif (btn==0) then //** Press button : Left Mouse Button : no window check 
    Cmenu = "MoveLink"   ; 

  elseif (btn==1000) then //** Press button : Left Mouse Button : no window check 
    Cmenu = "Smart Move"   ; 
  
  //**-------------------------------------------------------------    
  elseif (btn==10) & (win==curwin) then //** "Left Mouse Double Click" in the current Scicos window
    Cmenu="Open/Set"  //** Possible cases : 1 - Void (empty)
                      //**                  2 - Block
                      //**                  3 - Link
                      //**                  4 - Super Block
		      //**                  5 - Text   
  //** ----------------------------------------------------------- 
  elseif (btn==10) & (win<>curwin) then //** Left Mouse Button : Double click 
    jj = find(windows(:,2)==win)        //** not in current Scicos windows (eg palette or navigator)
    //** if jj is NOT empty means that you are in a Palette or in a Navigator  
    if jj <> [] then
      if or(windows(jj,1)==100000) then
        Cmenu = "Open/Set"  //double click in the navigator: mode open-set --> Navigator Window
       else
	Cmenu = "Duplicate" //** Double Click In a Palette windows ---> jump to Duplicate  
      end
    else
      Cmenu=[]; %pt=[]; //** otherwise, clear state variable
    end
  
  //** ----------------------------------------------------------- 
  
  //**--------------------- RIGHT MOUSE BUTTON -> POPUP -------------------------------------
  elseif or( btn==[2 5 12] ) then  //** any RIGHT mouse button events (click, press, d.click)
    Cmenu = "Popup";               //** means a popup request 
    return         ; //** --> EXIT to 'Popup' execution 
  //**---------------------------------------------------------------------------------------    
  
  elseif btn == -2 then  // Dynamic Menu (top of window) mouse selection
    win = curwin ;
    //** the format of the 'str' callback string is :
    //** "execstr(<Name_of_menu>_<win_id>(<menu_index>)),  e.g. "execstr(Diagram_1000(1))"
    //** <Name_of_menu> : is the label at the top of menu selection (static label present on the window) e.g. "File"
    //** <win_id>       : is the window id Scilab number (eg 1000 for the main SCICOS window)
    //** <menu_index>   : is the numeric index of the menu selected (in case of multiple menu).
    if strindex(str,'_'+string(curwin)+'(')<>[] then // str contains the information of the Scicos dynamic menu selection
      %pt=[] ; //** empty variable: no information about mouse position inside a dynamic menu  
      //** Cmenu is empty ( [] )
      //**      execstr( <Diagram_1000(1)>  )
      execstr('Cmenu='+part(str,9:length(str)-1));   //**
      //**  Cmenu = menus('Diagram')(1)      ; //** needs explanation  
      execstr('Cmenu='+Cmenu) ;
      //**  At the end 'Cmenu' contains the string show in the dinamic selection menu (e.g. "Replot")   
      return ; //** ---> EXIT POINT  
    else // click in an other dynamic menu
      execstr(str,'errcatch'); //** error handling 
      return ; //** ---> EXIT POINT     
    end
    
  //**-------------------------------------------------------------    
   
    //** Keys combos, mouse/key combos and sigle key shortcut
  elseif btn > 31 then //** [CTRL] + [.] combination  

    //** ------ Key combos ------------------------------------ 
        
    // [CRTL]+[x] | [Delete]   | [Backspace]  --> Cut
    if (btn==1120)|(btn==65288)|(btn==65535) then //** [CRTL]+[x] --> Cut
      Cmenu="Cut"; %pt=[];
    
    elseif (btn==1099) then            //** [CTRL]+[c] --> Copy
      Cmenu="Copy"; %pt=[];
    
    elseif (btn==1118) then            //** [CTRL]+[v] --> Paste 
      Cmenu="Paste"; %pt = [xc,yc] ;   //** acquire the position for the "Paste"
    
    elseif (btn==255) then             //** Backspace  --> Delete (erase) object          
      Cmenu="Delete"; %pt=[];
    
    //** ----- Mouse + Keyb. combos ---------------------------  
    elseif (btn==1003) | (btn==2003) then  //** [CTRL]+[Left btn click] OR [Shift]+[Left btn click]
      Cmenu="CtrlSelect";                  //** Multiple, additive object selection 
    
    elseif (btn==1122) then                //**  [CTRL]+[z] --> Undo 
      Cmenu="Undo";
    //**-------------------------------------------------------
    
    elseif (btn==1115) then                //** [CRTL]+[s] --> Save the diagram 
      Cmenu="Save";

    elseif (btn==1097) then                //** [CRTL]+[a] --> Select all 
      Cmenu="Select All";

    
    //**-------------------------------------------------------
    else
    //** ---- Shortcut ----------------------------------------
      Cmenu = %tableau( min (100,btn-31) ); //** (global) %tableau is the standard single key shorcut conversion
                                            //**          table; input (key_code-31) ; output: a string
                                            //** it uses "%scicos_short" preloaded variable 
      if Cmenu==emptystr() then  //** if the strings is empty means 
         Cmenu=[]; %pt=[];       //** that no single key shorcut is associated            
      end
             
    end //** end of keyboard combinations 
  
  end //** end of cosclik() filter 

endfunction
