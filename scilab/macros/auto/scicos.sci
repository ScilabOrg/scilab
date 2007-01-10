function [scs_m,newparameters,needcompile,edited] = scicos(scs_m,menus)
//** INRIA
//**
//** 20 dec 2005: Code modified by Simone Mannori
//** 06 Feb 2006 : restart of the mods job :
//**
//** 22 May 2006 : New restart
//**
//** 19 Jun 2006 : Last restart
//**
//** 10 Jul 2006 : looking for a residual oldgraphics instruction
//**
//**  1 Set 2006 : SCICOS menu sub-system
//** Comments & mods by Simone Mannori
//**
//**----------------------------------------------------------------------------------------
// scicos - block diagram graphic editor
// %SYNTAX
// scs_m=scicos(scs_m,job)
// %PARAMETERS
// scs_m    : scilab list, scicos main data structure
//      scs_m.props contains system name and other infos
//      scs_m.objs(i) contains description of ith block diagram element
// menus : vector of character strings,optional parameter giving usable menus
// !
//--------------------------------------------------------------------------------------------
// Copyright INRIA

//** Check for Scilab "command line mode" that does not support SCICOS 
noguimode = find(sciargs()=="-nogui");
if (noguimode <>[]) then
   clear noguimode
   warning(" Scilab in no gui mode : Scicos unavailable");
   abort;
end;

clear noguimode

[lhs,rhs] = argn(0) ;

//** ----------------------------- Check the recurring calling level of scicos_new -------------------------------

//check if superblock editing mode
[%ljunk, %mac] = where() ; //** where I am ?
                           //** looking for the position inside the tree
slevel = prod ( size ( find ( %mac=='scicos') ) ) ; //** "slevel" is the superblock level
super_block = slevel > 1 ; //** ... means that the actual SCICOS is a superblock diagram

//**--------------------------------------------------------------------------------------------------------------
//** Check about the initialization of the SCICOS internal datastructures that require init in case of
if ~super_block then

  //**------------------------------------------------------------------------------------------------------------
  // Check and define SCICOS palette , menu , shortcut , display mode , palette libraries
  if exists('scicos_pal')==0 | exists('%scicos_menu')==0 | exists('%scicos_short')==0 | ...
       exists('%scicos_display_mode')==0| exists('scicos_pal_libs')==0 then

    [scicos_pal_d, %scicos_menu_d, %scicos_short_d, %scicos_help_d,...
         %scicos_display_mode_d, modelica_libs_d, scicos_pal_libs_d] = initial_scicos_tables() ;

    if exists('scicos_pal')==0 then
      x_message(['scicos_pal not defined'; 'using default values'])
      scicos_pal = scicos_pal_d ;
    end

    if exists('%scicos_menu')==0 then
      x_message(['%scicos_menu not defined'; 'using default values'])
      %scicos_menu = %scicos_menu_d ;
    end

    if exists('%scicos_short')==0 then
      x_message(['%scicos_short not defined'; 'using default values'])
      %scicos_short = %scicos_short_d ;
    end

    if exists('%scicos_help')==0 then
      x_message(['%scicos_help not defined'; 'using default values'])
      %scicos_help = %scicos_help_d ;
    end

    if exists('%scicos_display_mode')==0 then
      x_message(['%scicos_display_mode not defined'; 'using default values'])
      %scicos_display_mode = %scicos_display_mode_d ;
    end

    if exists('modelica_libs')==0 then
      x_message(['modelica_libs not defined'; 'using default values'])
      modelica_libs = modelica_libs_d ;
    end

    if exists('scicos_pal_libs')==0 then
      x_message(['scicos_pal_libs not defined'; 'using default values'])
      scicos_pal_libs = scicos_pal_libs_d ;
    end

  end //** ... of the initialization variable
  //**-----------------------------------------------------------------------------------------------------------------------

  //** 'CmenuTypeOneVector' store the list of the commands/function to be called that require both 'Cmenu' AND '%pt'
  // menus of type 1 (require %pt)
   CmenuTypeOneVector =..
     ['Region to Super Block', 'Click, drag region and click (left to fix, right to cancel)'        ;
      'Smart Move',            'Click object to move, drag and click (left to fix, right to cancel)';
      'Move',                  'Click object to move, drag and click (left to fix, right to cancel)';
      'Duplicate',             'Click on the object to duplicate, drag, click (left to copy, right to cancel)';
      'Duplicate Region',      'Duplicate Region: Click, drag region, click (left to fix, right to cancel)';
      'Replace',               'Click on new object , click on object to be replaced';
      'Align',                 'Click on an a port , click on a port of object to be moved';
      'Link',                  'Click link origi-n, drag, click left for final or intermediate points or right to cancel';
      'Delete',                'Delete: Click on the object to delete';
      'Delete Region',         'Delete Region: Click, drag region and click (left to delete, right to cancel)';
      'Flip',                  'Click on block to be flipped'      ;
      'Open/Set',              'Click to open block or make a link';
      'MoveLink',              ''                                  ;
      'SelectLink',            ''                                  ;
      'CtrlSelect',            ''                                  ;
      'SelectRegion',          ''                                  ;
      'Popup',                 ''                                  ;
      'Label',                 'Click block to label';
      'Get Info',              'Click on object  to get information on it';
      'Code Generation',       'Click on a Superblock (without activation output) to obtain a coded block!' ;
      'Icon',                  'Click on block to edit its icon';
      'Color',                 'Click on object to paint';
      'Help',                  'Click on object or menu to get help'
      'Identification',        'Click on an object to set or get identification';
      'Resize',                'Click block to resize';
      'Block Documentation',   'Click on a block to set or get it''s documentation'
     ]
  //

  //**--------------------------------------------------------------------------------------

  //** initialize the "scicos_contex" datastructure (Scilab script inside SCICOS simulation)
  if ~exists('%scicos_context') then
    %scicos_context = struct() ;
  end

  //**--------------------------------------------------------------------------------------

  //** initialize a "scicos_debug_gr" variable for debugging editor
  if ~exists('%scicos_debug_gr') then
    %scicos_debug_gr = %f;
  end

  //** initialize a "scicos_with_grid" variable for drawing a grid
  if ~exists('%scicos_with_grid') then
    %scicos_with_grid = %f;
    //** %scs_wgrid(1:2) : space of grid
    //** %scs_wgrid(3) : color
    %scs_wgrid = [10;10;12];
  end

  //** Scicos internal function (scicos/routines/permuteobj) override in order to assure full
  //** compatibility with NGI (J.B. Silvy)
  swap_handles=permutobj; //TO be removed in scilab 5

  //intialize lhb menu
  %scicos_lhb_list = list()
  %scicos_lhb_list(1) = list('Open/Set',..
			  'Cut',..
			  'Copy',..
			  'Smart Move'  ,..
			  'Move'  ,..
			  'Duplicate',..
			  'Delete',..
			  'Link',..
			  'Align',..
			  'Replace',..
			  'Flip',..
			  list('Properties',..
			       'Resize',..
			       'Icon',..
			       'Icon Editor',..
			       'Color',..
			       'Label',..
			       'Get Info',..
			       'Identification',..
			       'Documentation'),..
			  'Code Generation',..
			  'Help');
			  
   %scicos_lhb_list(2)=list('Undo','Paste','Palettes','Context','Add new block',..
			  'Duplicate Region','Delete Region','Region to Super Block',..
			  'Replot','Save','Save As',..
			  'Load','Export','Quit','Background color','Aspect',..
			  'Zoom in',  'Zoom out','Help');

   %scicos_lhb_list(3)=list('Copy','Duplicate','Duplicate Region','Help');

 if exists('scicoslib')==0 then 
   load('SCI/macros/scicos/lib') ; //** load all the libraries relative to the palettes
 end

 exec(loadpallibs,-1) // to load the palettes libraries

 prot=funcprot();funcprot(0);
 exec(SCI+'/macros/util/ged.sci',-1); //to load functions of ged in scicos
 funcprot(prot)

end //** end of the main if() not superblock

//** ------------------------------------ End the NOT-Superbloc initialization and check ----------------------------------------

scicos_ver = 'scicos4.0.1' // set current version of scicos

Main_Scicos_window = 1000 ; //** set default value of the main scicos window

//Initialisation
newparameters = list() ;
enable_undo = %f
edited = %f
needreplay = %f
%path='./'
%exp_dir = PWD

global %tableau

if ~super_block then // init of some global variables
  %zoom    = 1.4      ; //** original value by Ramine
  pal_mode = %f       ;  // Palette edition mode
//  newblocks=[] // table of added functions in pal_mode
  super_path=[] // path to the currently opened superblock

  scicos_paltmp = scicos_pal ;

  if execstr('load(''.scicos_pal'')','errcatch')==0 then
    scicos_pal = [scicos_paltmp;scicos_pal]
    [%junk,%palce] = gunique(scicos_pal(:,2))
    %palce = -sort(-%palce);
    scicos_pal = scicos_pal(%palce,:);
  end

  execstr('load(''.scicos_short'')','errcatch')  // keyboard shortcuts

end
//

if rhs>=1 then //** scicos_new(...) is called with some arguments

  if type(scs_m)==10 then //diagram is given by its filename
    %fil=scs_m
    alreadyran=%f
    [ok,scs_m,%cpr,edited] = do_load(%fil,'diagram')
    if ~ok then return,end

    if size(%cpr)==0 then
      needcompile=4
      %state0=list()
    else
      %state0=%cpr.state;
      needcompile=0
    end

  else //diagram is given by its data structure

    if ~super_block then 
      %cpr=list(); needcompile=4 ; alreadyran=%f , %state0=list()
    end

  end

else //** scicos_new() is called without arguments (AND - implicitly - is NOT a superblock)
//** ----------- Normal : not a superblock -----------------  
//** ------------- NORMAL OPENING OF A BRAND NEW GRAPHICS WINDOW--------------------------------

  gh_Main_Scicos_window = scf(Main_Scicos_window); //** new way to open a brand new graphics windows  

  scs_m = scicos_diagram() ;

  %cpr = list(); needcompile = 4; alreadyran = %f; %state0 = list() ;

end

//

if typeof(scs_m)<>'diagram' then error('first argument must be a scicos diagram'),end


//**-------------------------------------------------------------------------------------------
//** Dynamic menu preparation
//**
%cor_item_exec = []; //** init

//** scan all the "%scicos_menu" an load "%cor_item_exec" ; dispose the first string (2:$) because
//** is the name of the dynamic menu
for %Y=1 : length(%scicos_menu)
  %cor_item_exec = [%cor_item_exec, %scicos_menu(%Y)(2:$)] ;
end

%cor_item_exec = %cor_item_exec'; //** transpose the vector
%R = %cor_item_exec; 
%R = stripblanks(%R)+'_' ; //** delete all the blanks and add an underscore at the end "Save As PAlette" -> "SaveAsPalette_"
%R = strsubst(%R,'/','') ; //** delete "/"
%R = strsubst(%R,' ','') ; //** delete " "
%R = strsubst(%R,'.','') ; //** delete "."
%R = strsubst(%R,'-','') ; //** delete "-"
%cor_item_exec = [%cor_item_exec, %R]; //** create the two column matrix [<MenuName> <FunctionToCall>]

// add fixed menu items not visible
%cor_item_exec = [%cor_item_exec;
                  'Link'        , 'Link_'         ;
		  'Open/Set'    , 'OpenSet_'      ;
		  'MoveLink'    , 'MoveLink_'     ;
		  'SelectLink'  , 'SelectLink_'   ;
		  'CtrlSelect'  , 'CtrlSelect_'   ;
		  'SelectRegion', 'SelectRegion_' ;
	          'Popup'       , 'Popup_'       ];

menus = tlist('xxx')

for %Y=1:length(%scicos_menu)
  menus(1)  = [menus(1), %scicos_menu(%Y)(1)];
  menus($+1)= %scicos_menu(%Y)(2:$);
end

for %Y=1:length(%scicos_menu)
  %R = %scicos_menu(%Y);
  %w = 'menus('''+%R(1)+''')('+ string(1:(size(%R,'*')-1)) + ')';
  execstr(%R(1)+ '=%w;');
end

//----------------------------------------------------------------
// single key shortcut: keyboard definition
%tableau = emptystr([1:100]);
for %Y = 1 : size(%scicos_short,1)
  %tableau(-31+ascii(%scicos_short(%Y,1)))=%scicos_short(%Y,2);
end
//----------------------------------------------------------------

// viewport
options = scs_m.props.options

// solver
%scicos_solver = scs_m.props.tol(6)

//**-------------------------- I'm NOT inside a superblock  -----------------------------------------------
if ~super_block then

  gh_current_window = scf(Main_Scicos_window);
  curwin = get ( gh_current_window, "figure_id") ;

  palettes = list();
  noldwin = 0
  windows = [1 curwin]
  pixmap = %scicos_display_mode
  //

  if ~exists('%scicos_gui_mode') then
    if with_tk() then %scicos_gui_mode=1, else %scicos_gui_mode=0, end
  end
  //%scicos_gui_mode=0

  if %scicos_gui_mode==1 then
    prot=funcprot();
    funcprot(0);
    getfile  = tk_getfile;
    savefile = tk_savefile;
    getcolor = tk_getcolor;
    if MSDOS then
      getvalue = tk_getvalue;
      mpopup   = tk_mpopup;
      choose   = tk_choose;
    else
      mpopup = tk_mpopupX;
      deff('x=choose(varargin)',...
             'x=x_choose(varargin(1:$))');
    end
    funcprot(prot);
  else
    deff('x=getfile(varargin)','x=xgetfile(varargin(1:$))');
    savefile = getfile;
    deff('Cmenu=mpopup(x)','Cmenu=[]')
    deff('x=choose(varargin)','x=x_choose(varargin(1:$))');
  end //** of %scicos_gui_mode==1
//
else //** super block case

  noldwin = size(windows,1)           ;
  windows = [windows ; slevel curwin] ;
  palettes = palettes                 ;

end //** end of not superblock

//** --------------------------------------- GRAPHICS INITIALIZATION -----------------------------------
//** This section is executed in any case
//   initialize graphics

clf(); //** clear current graphic window and delete all his children

//** this handle acquisition works for both MainScicosWindow and SuperBlock window
gh_current_window = gcf() ; //** get the current graphics window
gh_current_window.pixmap = "on";
if ~set_cmap(options('Cmap')) then // add colors if required
  options('3D')(1) = %f //disable 3D block shape
end

set_background() ; //** -->

pwindow_set_size() //** phisical window set size --> ok

window_set_size()  //** virtual window set sixe --> ok

//** Add the "_" character at the end of menu to call the corresponding function
for %Y=1:length(%scicos_menu)
    execstr( %scicos_menu(%Y)(1)+'_'+string(curwin)+'='+%scicos_menu(%Y)(1) )
end

menu_stuff(); //** delete the default menus in the graphics window and
              //** create the SCICOS menu using the 'menus' data structure
              //** and the macro "

//** The expandend superblock windows does not have the [stop] simulation button
if ~super_block then
  delmenu (curwin,'stop')  ;
  addmenu (curwin,'stop', list(1,'haltscicos')) ;
  unsetmenu(curwin,'stop') ;
else
  unsetmenu(curwin,'Simulate') ;
end

//** ----------------------------------------------------------------------------------------------

//** ---------------- This piece of code is not very clear ????????????????????????????????????????

//set context (variable definition...)
if type(scs_m.props.context) == 10 then

  gh_percent_now_win = gcf(); //** save current figure handle

  [%scicos_context,ierr] = script2var(scs_m.props.context,%scicos_context)
  //for backward compatibility for scifunc
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
    message(['Error occur when evaluating context:' lasterror() ]) ;
  else
    deff('%fonct()',scs_m.props.context)
    %outfun = macrovar(%fonct);
    //perform eval only if context contains functions which may give
    //different results from one execution to next
    if or(%outfun(4)=='rand')|or(%outfun(4)=='exec')|or(%outfun(4)=='load') then
      disablemenus() ;
       [scs_m, %cpr, needcompile, ok] = do_eval(scs_m, %cpr);
      enablemenus() ;
    end
  end

  scf(gh_percent_now_win); //** restore current figure handle
else

  scs_m.props.context=' '

end
//** -------------------- end of the very obscure code ---------------------------------------------
//**
//** -----------------------------------------------------------------------------------------------
//** Begin of the command interpreter loop

// state machine variables

%pt = [];

Cmenu = [];

%win = curwin;

//state machine variables windowish behavior

//** 'Select' 'Select_back' are matrix; each line is [object_id win_id]; 'object_is" is the same INDEX of 'scs_m'
//** and 'win_id" is the Scilab window id. Multiple selection is permitted.
Select = []      ; //** 
Select_back = [] ;

%ppt = []; //** used to store last click position for "Paste" operation 

Clipboard = []; //** used in Copy Cut and Paste function 

drawobjs(scs_m) ; //** draw the full diagram


//** ------------------------ Command Interpreter / State Machine / Main Loop ----------------------------

while ( Cmenu <> 'Quit' ) //** Cmenu -> exit from Scicos

//** ---------------------------------------------------------------// Used for "dynamic" stack allocation 
  [%stack] = stacksize()                                            // Automatically increase Scilab stack  
  if %stack(2)/%stack(1)> 0.3 then                                  // size for big diagram
    stacksize(2*%stack(1))                                          //
    disp('stacksize increased to '+string(2*%stack(1)))             //
  end                                                               //
//** ---------------------------------------------------------------

  //** This code fragment filter out a previus selection on a window NOT present in the
  //** actual Scilab active window list "winsid" (e.g. the window has been closed).   
  if Select<>[] then
    if ~or(Select(1,2) == winsid()) then Select=[],end
  end

  [CmenuType, mess] = CmType(Cmenu); //** recover command type and message  

  xinfo(mess); //** show the message associated to the command 
  
  //** clear the %pt information for backward compatibility
  //** if 'Cmenu' is empty (no command) but '%pt' is not , it is better to clear '%pt'
  if ( Cmenu == [] & %pt <> []  ) then %pt=[]; end

  //** if 'Cmenu' is NOT empty and 'CmenuType' is "0" I don't' need '%pt' then clear '%pt'
  if ( Cmenu<> [] & CmenuType==0) then %pt=[]; end

  //** if 'Cmenu' is NOT empty and 'CmenuType' is "1" and there is at least one object selected 
  if (Cmenu<>[] & CmenuType==1 & %pt==[] & Select<>[]) then
       [%pt,%win] = get_selection(Select) //** recover the %pt and %win from 'Select'
  end

  //** if no command is issued "Cmenu==[]" or
  //**    CmenuType==1 and no %pt information and no object selected 
  if ( Cmenu==[] | (CmenuType==1 & %pt==[] & Select==[]) ) then
      //** I'm not ready to exec a command: I need more information using cosclik()
      [btn_n, %pt_n, win_n, Cmenu_n] = cosclick() ; //** <-- The input function <------

      //** in the case of 'SelectLink'| Cmenu_n=='MoveLink' if previous Cmenu<>[] & CmenuType==1 operation
      //** is not completed just recover and use %pt = %pt_n and forget Cmenu_n
      //** otherwise update both Cmenu and %pt
      if (Cmenu_n=='SelectLink'| Cmenu_n=='MoveLink') & Cmenu<>[] & CmenuType==1 & %pt==[] then
	if %pt_n<>[] then %pt = %pt_n; end
      else
        if Cmenu_n<>[] then Cmenu = Cmenu_n; end
        if %pt_n <> [] then %pt = %pt_n;     end
	disp("Cmenu="); disp(Cmenu); disp("%pt=") ; disp(%pt) ; 
      end

      %win = win_n

  else
  //** I'm ready to exec a command 
      %koko = find( Cmenu==%cor_item_exec(:,1) ); //** find the command in the list "%cor_item_exec"
      //** if the command is found 
      if size(%koko,'*') == 1 then

        Select_back = Select; //** save the selected object list 

        //** Command execution 
	//** Don't ever think to touch this line of code ;)
	execstr('exec('+%cor_item_exec(%koko,2)+',-1)') ; //** call the function that
                                                          //** exec the desired action
	//** unselect ALL the previous selected object and select again the actually selected object
	//** because some command (operation) could select/unselect some object (the mods are registered
	//** in the "Select" variable)
	if or(Select<>Select_back) then
	  // Select_back: objects to unselect
	  // Select     : object to select
	  drawlater();
	    selecthilite(Select_back, 'off') ; // unHilite previous objects
	    selecthilite(Select, 'on') ;       // Hilithe the actual sected object
 	  drawnow(); show_pixmap() ; //** refresh, on screen
	end
	
      else
      //** if the command is not valid clear the state variable 
         Cmenu=[]; %pt=[]
      end //** a valid/invalid command to exec
      
   end //** not ready / ready to exec a command 

end //**--->  end of the while loop: the only way to exit is with the 'Quit' command  -------------------------------

do_exit() ; //** this function is executed in case of 'Quit' command

endfunction

//** ----------------------------------------------------------------------------------------------------------------
//*******************************************************************************************************************

function [itype, mess] = CmType(Cmenu)
  //** look inside "CmenuTypeOneVector" if the command is type 1 (need both Cmenu and %pt)
  k = find (Cmenu == CmenuTypeOneVector(:,1)); 
  if k==[] then //** if is not type 1 (empty k)
    itype = 0 ; //** set type to zero
    mess=''   ; //** set message to nothing 
    return    ; //** --> EXIT point : return back 
  end

  if size(k,'*')>1 then //** if found more than one command 
    message('Warning '+string( size(k,'*'))+' menus have identical name '+Cmenu);
    k=k(1); //** ? 
  end

  itype = 1 ; 

  mess = CmenuTypeOneVector(k,2) ; 
  
endfunction

//** ----------------------------------------------------------------------------------------------------------------

function [x,k] = gunique(x)
    [x,k]=gsort(x);
    keq=find(x(2:$)==x(1:$-1))
    x(keq)=[]
    k(keq)=[]
endfunction

//** ----------------------------------------------------------------------------------------------------------------

function selecthilite(Select, flag)  // update the image

  gh_winback = gcf() //** get the window

  for i=1:size(Select,1)

    [junk, win, o] = get_selection(Select(i,:))

    gh_curwin = scf(win); //** select current window

    o_size = size ( gh_curwin.children.children ) ;
    //** initial size
    //** at this point I need to build the [scs_m] <-> [gh_window] datastructure
    //** I need an equivalent index for the graphics

    //** "k" is the object index in the data structure "scs_m"
    //** compute the equivalent "gh_k" for the graphics datastructure
    k = Select (i,1)
    //gh_k = o_size(1) - k + 1 ; //** semi empirical equation :)
    gh_k = get_gri(k,o_size(1)); //** Alan -

   if gh_k>0 then //** Alan - to disable some crash when we have delete obj
      //** get the handle of the graphics entity to modify
      gh_obj = gh_curwin.children.children(gh_k);
      //** update mark_mode property
      gh_obj.children(1).mark_mode = flag  ;
   end

  end //** end the for(..) loop

  scf(gh_winback)

endfunction

//**---------------------------------------------------------------------------------------------------------------------