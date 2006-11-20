function [btn ,%pt ,win ,Cmenu ] = cosclick(flag)
//** INRIA
//** Comments by Simone Mannori 
//** btn : button / event id (numeric code)
//** %pt : mouse position[x,y] of the event
//** win : Scilab/Scicos windows whre the event occour
//** 
//** 12 July 2006 : Revision
//**

  [lhs,rhs] = argn(0) ;
  
  Cmenu_orig = Cmenu ; //** save the old state 
  
  Cmenu=[]; %pt=[]; btn = 0 ; //** Clear variables
  
  if ~or( winsid()==curwin ) then //** if the current window (curwin is a semiglobal variable) is NOT  
       //** win=xget('window') ;       //** a Scicos window, then exit with "Quit"
       gh_win = gcf(); //** get the handler of the curent window 
       disp("...cosclic.sci...|:");pause; //** debug only 
       Cmenu='Quit'       ;
       return             ; 
  end   
  
  if rhs==1 then
    [btn, xc, yc, win, str] = xclick(flag) //** not used
  else
    [btn, xc ,yc ,win ,str ] = xclick()    //** <- This is used in the main scicos_new() loop:
  end                                 //**    CLEAR ANY PREVIOUS EVENT in the queue

  %pt = [xc,yc] ; //** acquire the position  
  
  disp(btn); //** debug
  
  //** -----------------------------------------------------------
  if or( btn==[2 5] ) then    //   button 3 ("right" mouse button) "pressed" OR "clicked"
    Cmenu='Popup';            //     means that it is a aux functions request
    return       ;     	      //** --> EXIT 
    
    //**------------------------------------------------------------
  elseif btn==-100 then  // The window has been closed 
    
    if win==curwin then  //** in the current window ? 
      Cmenu='Quit' ;
    else                 //** not the current window 
      Cmenu=[]     ;
      %pt=[]       ;
    end
    
    return  //** --> EXIT  
    
    //**-------------------------------------------------------------    
  elseif btn == -2 then  // click in a dynamic menu
    win = curwin ;
    disp("Dyn.Menu Callback string...");disp(str) ; //** debug only
    //** the format of the 'str' callback string is :
    //** "execstr(<Name_of_menu>_<win_id>(<menu_index>)) .... eg ... execstr(Diagram_1000(1))
    //** <Name_of_menu> : is the label at the top of menu selection (static allways present on the window) eg Diagram
    //** <win_id> :  is the window id number (eg 1000 for the main SCICOS window)
    //** <menu_index> : is the numeric index of the menu selected (in case of multiple menu).
    if strindex(str,'_'+string(curwin)+'(')<>[] then // str contains the information of the 
                                                     // click in a Scicos dynamic menu
      %pt=[] ; //** empty variable: no information about mouse position inside a dynamic menu  
      //** disp("Cmenu(0)=...");disp(Cmenu) ; //** debug only
      //** Cmenu is empty ( [] )
      //**      execstr( <Diagram_1000(1)>  )
      execstr('Cmenu='+part(str,9:length(str)-1));   //**
      //**  disp("Cmenu(1)=...");disp(Cmenu) ; //** debug only 
      //**  Cmenu = menus('Diagram')(1)      ; //** needs explanation  
      execstr('Cmenu='+Cmenu) ;
      //**  disp("Cmenu(2)=...");disp(Cmenu) ; //** debug only
      //**  At the end 'Cmenu' contains the string show in the dinamic selection menu (eg Replot   
      return ; //** ---> EXIT POINT  
    else // click in an other dynamic menu
      execstr(str,'errcatch')
      return ; //** ---> EXIT POINT     
    end
    
    //**-------------------------------------------------------------    
  elseif (btn==10) & (win<>curwin) then //** left button double click in a palette or navigator windows
    jj = find(windows(:,2)==win)
    //** if jj is empty means that you are in a Palette or in a Navigator  
    if jj <> [] then
      
      if or(windows(jj,1)==100000) then
        Cmenu = 'Open/Set'  //mode open-set (cliquer dans navigator) --> Navigator Window
       else
	Cmenu = 'Duplicate'  //** Double Click In a Palette windows ---> jump to do_duplicate ! 
      end
    
    else
      Cmenu=[];%pt=[];
    end
    
    //**-------------------------------------------------------------    
  elseif (btn==10) & (win==curwin) then //** Left Mouse button double click in the current windows
    Cmenu='Open/Set'                    //** 1 - empty
                                        //** 2 - block
                                        //** 3 - link
                                        //** 4 - super block        
					                  
     //**-----------------------------------------------------------
  elseif (btn==3) then     //** Left Mouse Button : Single click : no window check         
    Cmenu='SelectLink'
    
    //** -----------------------------------------------------------
  elseif  (btn==0) then  //** Left Mouse Button : Press button : no window check 
    Cmenu='MoveLink'
    
    //** -----------------------------------------------------------
  
    //** Keys combo, mouse/key combo and sigle key shortcut
  elseif btn > 31 then //** [CTRL] + [.] combination  

    if (btn==1120) | (btn==65288) then //** [CRTL]+[x] ... the other code ?
      Cmenu='Cut'; %pt=[];
    
    elseif (btn==1099) then            //** [CTRL]+[c]
      Cmenu='Copy';%pt=[];
    
    elseif (btn==1118) then            //** [CTRL]+[v]
      Cmenu='Paste';%pt=[];
    
    elseif (btn==255) then             //** code ?            
      Cmenu='Delete';%pt=[];
    
    elseif (btn==1003) | (btn==2003) then  //** [CTRL] + [Left btn click] OR [Shift] + [Left btn click]
      Cmenu="CtrlSelect";                  //** Multiple, additive object selection 
    
    elseif (btn==1122) then            //**  [CTRL]+[z]
      Cmenu="Undo"
    
    else
      Cmenu = %tableau( min (100,btn-31) ); //** (global) %tableau is the standard single key shorcut conversion
                                            //**          table; input (key_code-31) ; output: a string
                                            //** it uses "%scicos_short" preloaded variable 
					    
      if Cmenu==emptystr() then  //** if the strings is empty mean that there is no single key shorcut associated            
         Cmenu=[]; %pt=[];       //**             
      end
       
    end
  
  end

//
//  
//  %scicos_short  =
//  
// !a  Align      !
// !              !
// !d  Delete     !
// !              !
// !c  Duplicate  !
// !              !
// !m  Move       !
// !              !
// !u  Undo       !
// !              !
// !f  Flip       !
// !              !
// !o  Open/Set   !
// !              !
// !s  Save       !
// !              !
// !i  Get Info   !
// !              !
// !r  Replot     !
// !              !
// !l  Link       !
// !              !
// !q  Quit       !

endfunction
