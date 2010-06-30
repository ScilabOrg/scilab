//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ./license.txt
//

function [scs_m, newparameters, needcompile, edited] = scicos(scs_m)
//Copyright INRIA

// scicos - block diagram graphic editor
// %SYNTAX
// scs_m=scicos(scs_m)
// %PARAMETERS
// scs_m    : scilab list, scicos main data structure
//      scs_m.props contains system name and other infos
//      scs_m.objs(i) contains description of ith block diagram element
//      scs_m.version contains the number version of the diagram
// menus : vector of character strings,optional parameter giving usable menus
// !
//--------------------------------------------------------------------------------------------
// Copyright INRIA


// SCILAB 5.2 --> XCOS

error(msprintf(gettext("''scicos'' is no more available, please use ''xcos'' instead.\n")));

return

// SCILAB 5.2 --> XCOS

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//** Magic Global Variable for Diagram Browser and more
  global %scicos_navig
  global %diagram_path_objective
  global inactive_windows
  global Scicos_commands   // programmed commands
  global %tableau          //shortcuts
  global next_scicos_call
  //** "0" standard scicos oblique link ; "1" SL orthogonanal links
  global SL_mode ; SL_mode = %t ;
  global XcosClipboard  

  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //**-----------------------------------------------------------------------------------------
  //** Check for Scilab "command line mode" that does not support SCICOS
  //** This check verify if "scicos()" is started form a command line Scilab with no graphics
  //** support (almost obsolete function)

  noguimode = find(sciargs()=="-nogui");
  if (noguimode <>[]) then
    clear noguimode
    warning(_(" Scilab is in no gui mode : Scicos is unavailable"));
    abort ; //** EXIT form Scicos ()
  end;

  clear noguimode
  //**-----------------------------------------------------------------------------------------

  
  //** -------------------- Check the recurring calling level of scicos ----------------------
  [%ljunk, %mac] = where() ; //** where I am ?
  slevel = prod ( size ( find ( %mac=='scicos') ) ) ; //** "slevel" is the superblock level
  super_block = slevel > 1 ; //** ... means that the actual SCICOS is a superblock diagram

  //** -------------------- Main initialization -----------------------------------------------

  if ~super_block then  
    //**---- Scicos splash message 
    if next_scicos_call==[] then
      next_scicos_call = 1 ;
      verscicos = get_scicos_version() ;
      mprintf(_("Scicos version %s\n"+ ..
		" (adapted for Scilab 5 by The Scilab Consortium)\n"+ ..
                "Copyright (c) 1992-2009 Metalau project INRIA\n"+ ..
                "Licensed under the GNU Public License (GPL)\n"),part(verscicos,7:length(verscicos)))
    end

   
    //**---- Check and define SCICOS palette , menu , shortcut ,  palette libraries
    [scicos_pal, %scicos_menu, %scicos_short, modelica_libs, scicos_pal_libs,...
     %scicos_lhb_list, %CmenuTypeOneVector, %scicos_gif,%scicos_contrib, ..
     %scicos_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables() ;
    
    //**---- Set up navigation
    super_path    = [] // path to the currently opened superblock
    %scicos_navig = []
    inactive_windows = list(list(),[])
    Scicos_commands = [];
    
    //**---- Initialize context
    if ~exists('%scicos_context') then
      %scicos_context = struct() ;
    end

    //**---- Initialize  debugging editor
    if ~exists('%scicos_debug_gr') then
      %scicos_debug_gr = %f; //** debug mode : default is "%f"
    end

    //**---- prepare from and to workspace stuff
    xcos_workspace_init()
    
    //**---- Activate Graphics editor in Scicos
    prot = funcprot();funcprot(0);	//disable scilab function protection
    exec(SCI+'/modules/graphics/macros/ged.sci',-1);
    funcprot(prot)//restore scilab function protection

    //**----  Check and - eventually - load the Scicos function library
    if exists('scicos_scicoslib')==0 then
      load("SCI/modules/scicos/macros/scicos_scicos/lib") ;
    end
    
    //**----  Load library that contains the INTERFACING functions
    exec(loadpallibs, 1) 
    
    //**---- Magic Numbers
    Main_Scicos_window = 1000 ; //** set default value of the main scicos window
    %zoom    = 1.4      ; //** original value by Ramine
    pal_mode = %f       ;  // Palette edition mode







    TCL_EvalFile(SCI+"/modules/scicos/tcl/MIHM.tcl")



    //**---- Scilab 5 patch for font handling. This patch fix the "Symbol" font issue
//    scilab5fonts = xlfont() ; //** recover the full font list
//    xlfont(scilab5fonts(1), 1) ; //** substitute the font in position one
 
    
    //---- Load user's customization files
    //**  user defined palette relatives
    scicos_paltmp = scicos_pal ;
    if execstr('load(''.scicos_pal'')','errcatch')==0 then
      //** merge and check if the load has been positive
      scicos_pal = gunique(scicos_pal,scicos_paltmp); 
      scicos_pal = check_palettes_paths(scicos_pal)
    end
    //** load - if present - the used defined shortcuts
    execstr('load(''.scicos_short'')','errcatch') 


    //**---- Single key shortcut: keyboard definition
    %tableau = emptystr([1:100]);
    for %Y = 1 : size(%scicos_short,1)
      %tableau(-31+ascii(%scicos_short(%Y,1)))=%scicos_short(%Y,2);
    end
   
    //**---- Define some generic functions used by Scicos
    prot = funcprot();funcprot(0);
    //** allows to write inside the figure "user_data" field
    %diagram_i_h = generic_i_h 
    getfile  = uigetfile ; //** brand new aliases
    savefile = uiputfile ;

    mpopup = createpopup ;
    mdialog = x_mdialog  ;

    scicos_getvalue=scicos_getvalue
    choose=x_choose
    funcprot(prot);
    
    //**---- Set the graphic window
    gh_Main_Scicos_window = scf(Main_Scicos_window);
   
  end //End of the main initialization and check
  //**----------------------------------------------------------------------------------

  %diagram_open = %t     //** default choice
  if super_path<>[] then //** inside super block
    if isequal(%diagram_path_objective,super_path) then
      if %scicos_navig<>[] then
	%diagram_open = %t
	%scicos_navig = []
	gh_curwin = scf(curwin);
      end
    elseif %scicos_navig<>[] then
      %diagram_open = %f
    end
  end

 
  //** Initialisation  
  newparameters = list() ;
  enable_undo = %f
  edited = %f
  needreplay = %f
  %path='./'
  %exp_dir = PWD

  //**----------------------------------------------------------------------------------

 
  if argn(2)>=1 then //the argument specifies the initial diagram to edit
    
    if type(scs_m)==10 then // diagram is given by its filename (main diagram only)
      %fil = scs_m ;
      alreadyran = %f
      [ok,scs_m,%cpr,edited] = do_load(%fil,'diagram');
      if ~ok then return, end

      if size(%cpr)==0 then
	needcompile = 4 ;
	%state0     = list();
      else
	%state0 = %cpr.state;
	needcompile=0
      end
    else // diagram is given by its data structure (main or Super Block)
      if ~super_block then
	%cpr=list(); needcompile=4 ; alreadyran=%f , %state0=list()
      end
    end

  else //** scicos() is called without arguments (main call)
   //** In case a back up file exists
    ierr = execstr('load(TMPDIR+''/BackupSave.cos'')','errcatch')
    if ierr<>0 then
      //create an empty new diagram
      scs_m = scicos_diagram(version = get_scicos_version()) ;
      %cpr = list()    ;
      needcompile = 4  ;
      alreadyran = %f  ;
      %state0 = list() ;
    else
      load(TMPDIR+'/BackupInfo');
    end
  end
  
  //Check initial diagram validity
  if typeof(scs_m)<>'diagram' then
    error(msprintf(_("%s: First argument must be a Scicos diagram data structure\n"),'scicos'));
  end

  // viewport
  options = scs_m.props.options

  // solver
  %scicos_solver = scs_m.props.tol(6)

  //** ------- GRAPHICS INITIALIZATION: Palettes,  ---------
  if ~super_block then //main diagram
    curwin = Main_Scicos_window;
    //curwin = get ( gh_current_window, "figure_id") ;
    palettes = list();
    noldwin = 0      ;
    windows = [1 curwin] ;
  else //** super block case
    noldwin = size(windows,1)           ;
    windows = [windows ; slevel curwin] ;
    palettes = palettes                 ;
  end //** end of not superblock

  //**------------------------- CONTEXT -----------------------
  //set context (variable definition...)

  if type(scs_m.props.context) == 10 then //** if the variable is defined

    gh_percent_now_win = gcf(); //** save current figure handle

    [%scicos_context,ierr] = script2var(scs_m.props.context,%scicos_context)
    //for  compatibility with old  scifunc blocks
    if ierr==0 then
      %mm = getfield(1,%scicos_context)
      for %mi=%mm(3:$)
	ierr = execstr(%mi+'=%scicos_context(%mi)','errcatch')
	if ierr<>0 then
	  break
	end
      end
    end
    //end of for backward compatibility for scifunc

    if ierr  <>0 then
      messagebox([_("An error occur when evaluating context:"); lasterror() ],"modal") ;
    else
      //Check if it necessary to reevaluate block parameters 
      // it is the case if then context contains functions which may give
      //  different results from one execution to next
      deff('%fonct()',scs_m.props.context)
      %outfun = macrovar(%fonct);
      clear('%fonct')
      //** perform eval only 
      if or(%outfun(4)=='rand')|or(%outfun(4)=='exec')|or(%outfun(4)=='load') then
	DisableAllMenus()
	[scs_m, %cpr, needcompile, ok] = do_eval(scs_m, %cpr);
	EnableAllMenus()
      end
    end

    scf(gh_percent_now_win); //** restore current figure handle

  else

    scs_m.props.context = ' ' ; //** put the variable to empty state

  end
  //** ---------- End of "Context" handling and evaluation -----------

  //** Begin of the Main command interpreter loop

  //** Initial conditions
  Cmenu = []     ; //** valid command = empty
  %pt   = []     ; //** valid last mouse position = empty
  %win  = curwin ; //** curwin is dynamically modified if a superblock window is open
  %curwpar = []  ; // window dimensions

  //** 'Select' and 'Select_back' are matrix used to store selected objects
  //     Each line is:  [object_id win_id] : 
  //    "object_id" is the same INDEX used in "scs_m.obj"
  //    "win_id"    is the Scilab window id.
  //**  Multiple selection is permitted: each object is a line of the matrix.
  Select = []      ; 
  Select_back = [] ; 
  
  %ppt = []; //** used to store last valid click position for "Paste" operation

  //** ------------------- GRAPHICS INITIALIZATION ---------
   
  //** This section is executed in any case
  //   initialize graphics

  if %diagram_open then
    gh_current_window = gcf() ; //** get the current graphics window

    if (gh_current_window.user_data==[]) | (~isequalbitwise(gh_current_window.user_data(1),scs_m)) then
       %zoom = restore(gh_current_window,%scicos_menu, ~super_block) ;
       execstr('drawobjs(scs_m)', 'errcatch') ;
     else
       Select = gh_current_window.user_data(2) ;
       enable_undo = gh_current_window.user_data(3) ;
       scs_m_save  = gh_current_window.user_data(4) ;
       nc_save     = gh_current_window.user_data(5) ;
       show_window();
     end

  else //** diagram is NOT open

    if or(curwin==winsid()) then
      gh_current_window = scf(curwin) ;
      if (gh_current_window.user_data~=[]) & (isequalbitwise(gh_current_window.user_data(1),scs_m)) then
	Select = gh_current_window.user_data(2) ;
      end
    end

  end

//** --- End of initialization -----------------------------------------------------------



  //** -------------    M A I N    L O O P  -----------------------------
  //** --------- Command Interpreter / State Machine ------------
  while ( Cmenu <> "XcosMenuQuit" & Cmenu <> "XcosMenuLeave"  ) //** Cmenu -> exit from Scicos

    //** Dynamic stacksize for very big diagram           
    [%stack] = stacksize()                                
    if %stack(2)/%stack(1)> 0.3 then                      
      stacksize(2*%stack(1))                              
      disp("Stacksize increased to "+string(2*%stack(1))) 
    end                                                   
   

    if or(winsid()==curwin) then 
      //** Dynamic window resizing and centering -------------------------
      //Update the canvas size according to a possible size change of the
      //  physical figure.
      //This part should be pushed in a callback associated with the
      //  resize event (when available)
      winsize = gh_current_window.figure_size;
      axsize  = gh_current_window.axes_size;

      if or(winsize > axsize) then
        viewport = gh_current_window.viewport;
        viewport = max([0,0], min(viewport,-winsize+axsize));
        window_set_size(gh_current_window, viewport)
        drawnow(); //** update the diagram on screen
      end

      if edited then
	//** Update the figure size, canvas size, view port .., in scs_m.props
	//This part should be pushed in a callback associated with the
	//  close event (when available)
        scf(gh_current_window);
        gh_axes = gca();
        data_bounds = gh_axes.data_bounds;
	winpos   = gh_current_window.figure_position ;
        viewport = gh_current_window.viewport ;
	axsize   = gh_current_window.axes_size;
        %curwpar = [data_bounds(:)', axsize, viewport, winsize, winpos, %zoom];

	if ~isequal(scs_m.props.wpar, %curwpar) then
	  scs_m.props.wpar = %curwpar  // keep window dimensions
	end

      end //** edited section

    end //**... end of: Dynamic window resizing and centering ----------

    if %scicos_navig==[] then
      if Scicos_commands<>[] then
	execstr(Scicos_commands(1))
	Scicos_commands(1)=[]
      end
    end

    if Cmenu=="XcosMenuQuit" then
        break ; //** EXIT point ...
    end

    //**--------------------------------------------------------------------
    if %scicos_navig<>[] then //** navigation mode active
      
      //select the requested diagram or sub-diagram

      while %scicos_navig<>[] do
	if ~isequal(%diagram_path_objective,super_path) then
	  %diagram_open  = %f
          Select_back=Select
	  [Cmenu,Select] = Find_Next_Step(%diagram_path_objective, super_path)
	  if or(curwin==winsid()) & ~isequal(Select,Select_back) then
	    drawlater() ;
	    selecthilite(Select_back, "off") ; // unHilite previous objects
	    selecthilite(Select, "on") ;       // Hilite the actual selected object
	    drawnow() ;
          end

          if Cmenu=="XcosMenuOpenSet" then
	    ierr=0
	    execstr('ierr=exec(XcosMenuOpenSet,''errcatch'',-1)')
            //execstr('exec(OpenSet_,-1)')
	    //**---------------------------------------------------
	    if ierr<>0 then messagebox(lasterror(),"modal"),end
	    if isequal(%diagram_path_objective,super_path) then // must add after testing &%scicos_navig<>[]
	      if ~or(curwin==winsid()) then
		gh_current_window = scf(curwin);
                %zoom = restore(gh_current_window,%scicos_menu, ~super_block);
                execstr('drawobjs(scs_m)', 'errcatch') ;
		%scicos_navig =[];
		Select_back   =[];
                Select        =[];
	      else
		gh_current_window = scf(curwin);
	      end
	    else
	      if ~or(curwin==winsid())&%scicos_navig==[] then
		%scicos_navig = 1 ;
		%diagram_path_objective = [] ;
	      end
	    end
	  elseif Cmenu=="XcosMenuQuit" then
	    do_exit() ;
	    return ;
	  end
	  //**---------------------------------------------------
	else
	  %scicos_navig = [] ;
	end
      end //end  %scicos_navig<>[]

    else //** not in navigation mode
      %diagram_open = %t ;

      if ~or(curwin==winsid()) then
	gh_current_window = scf(curwin);
        %zoom = restore(gh_current_window,%scicos_menu, ~super_block)
        execstr('drawobjs(scs_m)', 'errcatch') ;
	Select_back = [] ;
        Select      = [] ;
      else
        gh_current_window = scf(curwin);
      end

      if Select<>[] then
	if ~or(Select(1,2) == winsid()) then
	  Select = [] ; //** imply a full Reset
	end
      end

      //**--------------------------------------------------------------------
      //** Command classification and message retrivial
      [CmenuType, mess] =  XcosGetMenuType(Cmenu); 
      xinfo(mess); //** show the message associated to the command

      //** ----------------- State variable filtering ----------------
      //** clear the %pt information for backward compatibility
      if ( Cmenu == [] & %pt <> []  ) then %pt=[]; end
      if ( Cmenu<> [] & CmenuType==0) then %pt=[]; end

      gh_current_window = scf(curwin);

      if (Cmenu<>[] & CmenuType==1 & %pt==[] & Select<>[]) then
	//** recover the %pt and %win from 'Select'
	[%win,junk_o,%pt] = get_selection(Select(1,:)) 
      end

      //** ---------------------- Main decisional section --------------------------------------

      if ( Cmenu==[] | (CmenuType==1 & %pt==[] & Select==[]) ) then
	//** if no command is issued  or CmenuType==1 and no %pt information and no object selected
	//** I'm not ready to exec a command: I need more information using cosclik()
        EnableAllMenus()
	[btn_n, %pt_n, win_n, Cmenu_n] = cosclick() ;
	
	if btn_n<>-1000 then  DisableAllMenus(),end
	
	if (Cmenu_n=="XcosMenuSelectLink" | Cmenu_n=="XcosMenuMoveLink") & Cmenu<>[] & CmenuType==1 & %pt==[] then
	  if %pt_n<>[] then %pt = %pt_n; end
	else
	  if Cmenu_n<>[] then Cmenu = Cmenu_n; end
	  if %pt_n <> [] then %pt = %pt_n;     end
	end
	%win = win_n
      else
	//** I'm ready to exec a command
	if exists(Cmenu) then
	  Select_back = Select; //** save the selected object list
          ierr = 0
	  //Preserve Cmenu label for error message (Cmenu is modified by exec)
	  CmenuLabel=XcosGetMenuLabel(Cmenu,%scicos_menu) 
	  
          //** Used for AGGRESSIVE DEBUG ONLY -->
	  //** exec(evstr(Cmenu),1)

          //** Used for standard DEBUG ONLY -->
          exec(evstr(Cmenu),-1); //** nothing is printed
          //** RELEASE --> Please reactivate the error catcher before final release
	  //** ierr=exec(evstr(Cmenu),'errcatch',-1)

	  if ierr > 0 then
	    messagebox([
		msprintf(_("An unexpected  error occurred while executing the menu\n""%s"":\n"),CmenuLabel)
		''
		lasterror()
		''
		_("Please report")
		       ],'Error','modal')
	    Select_back = [];
            Select = [] ;
	   
	    Cmenu = "XcosMenuReplot"
	  elseif or(curwin==winsid()) then
	    gh_current_window = scf(curwin);

            if ~isequal(Select,Select_back) then
              drawlater();
	      selecthilite(Select_back, "off") ; // unHilite previous objects
	      selecthilite(Select, "on") ;       // Hilite the actual selected object
              drawnow();
	    end

          else
	    if %scicos_navig==[] then // in case window is not open
	      %scicos_navig = 1
	      %diagram_path_objective = []
	    end
	  end

	else
	  //** if the command is not valid, clear the state variable
	  messagebox(msprintf(_("Requested action: %s is not available"),Cmenu),'modal') 
	  Cmenu = []; %pt = []
	end //** a valid/invalid command to exec

      end //** not_ready / ready ... to exec a command

    end //** not in navigation mode ???

  end //**--->  end of the while loop: exit with the 'Quit' OR 'Leave' commands

  //** if you are exited from the main loop with 'Quit'
  if Cmenu=="XcosMenuQuit" then
    //**  -------------- 'Quit' ------------------------------------
    //**  Quit exit from suberblock or go back to Scilab command line
    do_exit() ;
    
    if ~super_block then // even after quiting, workspace variables
                         // must be saved and be usable in Scilab
      if find(%mac=='scilab2scicos') ==[] then
	[txt,files]=returntoscilab()
	n=size(files,1)
	for i=1:n
	  load(TMPDIR+'/Workspace/'+files(i))
	  execstr(files(i)+'=struct('"values'",x,'"time'",t)')
	end
	execstr(txt)
      end

      mdelete(TMPDIR+'/BackupSave.cos') // no backup needed
      mdelete(TMPDIR+'/BackupInfo')
      clearglobal XcosClipboard
      clearglobal Scicos_commands
      clearglobal %tableau
      clearglobal %scicos_navig
      clearglobal %diagram_path_objective
      close_inactive_windows(inactive_windows,[])
      clearglobal inactive_windows

      //** restore the original Scilab 5 font list before exit
      //** xlfont(scilab5fonts(2),1);
      xlfont('reset');
      //** TO DO
      //** For future implementation .....
      //** This is the place of the windows cleaning section

    end

  elseif Cmenu=="XcosMenuLeave" then
   //**  -------------- 'Leave' ------------------------------------
   //**  "Leave" go to Scilab leaving Scicos active in the background
   //**  and use "seteventhandler" do to the job
    ok = do_save(scs_m,TMPDIR+'/BackupSave.cos')
    if ok then  //need to save %cpr because the one in .cos cannot be
                //used to continue simulation
      if ~exists('%tcur') then %tcur=[];end
      if ~exists('%scicos_solver') then %scicos_solver=0;end
      save(TMPDIR+'/BackupInfo', edited,needcompile,alreadyran, %cpr,%state0,%tcur,..
                                            %scicos_solver,inactive_windows)

      OpenPals=windows(find(windows(:,1)<0),2 )  //close palettes
      for winu=OpenPals'
        if or(winu==winsid()) then
          gh_del = scf(winu) ; //** select the 'winu'window and get the
                               //handle
          delete (gh_del)   ; //** delete the window
        end
      end
    end

    if ~ok then
      messagebox(msprintf(_("Problem saving a backup; I cannot activate Scilab\n"+..
			    "Save your diagram scs_m manually.")),"modal");
      pause ;
    end

    AllWindows = unique([windows(:,2);inactive_windows(2)])
    AllWindows = intersect(AllWindows',winsid())

    //** activate the same event handler for all the active Scicos windows
    for win_i= AllWindows
      scf(win_i); //** select
      seteventhandler("scilab2scicos"); //** activate the event handler
    end

    save(TMPDIR+'/AllWindows',AllWindows)
    //   scf(0)  // to protect scicos windows when in Scilab
    mprintf(_("To reactivate Scicos, click on a diagram or type ""scicos();""\n"))


    if edited then
      mprintf(_("Your diagram is not saved. Do not quit Scilab or "+...
	      "open a new Scicos diagram before returning to Scicos."))
    end
    // prepare from and to workspace stuff

    if find(%mac=='scilab2scicos') ==[] then
      [txt,files] = returntoscilab();
      n = size(files,1)
      for i=1:n
	load(TMPDIR+'/Workspace/'+files(i))
	execstr(files(i)+'=struct('"values'",x,'"time'",t)')
      end
      execstr(txt);
    end

  end

endfunction //** scicos(); end here :) : you had a good day

//** ----------------------------------------------------------------------------------------------------------------

function uni = gunique(m1,m2)
// Used to eliminate duplicate palette item(s)
  uni = [m1;m2] ; //** merge the two matrix
  [j,ind] = unique(uni(:,1) + uni(:,2)) ; //** remove the duplicate palette name
  uni = uni(-gsort(-ind),:);  //** reorder the merged palette without duplicate
endfunction


//**---------------------------------------------------------------------------------------------------------------------

function scicos_pal = check_palettes_paths(scicos_pal)
  toremove=[]
  for k=1:size(scicos_pal,1)
    if fileinfo(scicos_pal(k,2))==[] then toremove=[toremove k],end
  end
  //remove invalid ones out of scicos_pal
  if toremove<>[] then
    rmpal=scicos_pal(toremove,:)
    rmpal(:,1)=part(rmpal(:,1),1:max(length(rmpal(:,1))))
    messagebox([_("Following palette(s) ignored (associated file(s) no more exist):");
		' '
		rmpal(:,1)+':  '+rmpal(:,2)
		' '
		msprintf(_("To avoid this message, please update the ""%s.scicos_pal"" file."),pwd()+filesep())],"modal")
    scicos_pal(toremove,:)=[];
  end
endfunction

