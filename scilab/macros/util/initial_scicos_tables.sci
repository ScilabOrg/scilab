function [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ...
	  %scicos_display_mode, modelica_libs,scicos_pal_libs, ...
	  %scicos_lhb_list, %CmenuTypeOneVector ] = initial_scicos_tables()
//**
//** INRIA
//** Comments by Simone Mannori
//** 04 Sep 2006 : updated file with the last "scicos_new" mods
//**
//** 30 August 2007: some data structure are moved from scicos.sci here.
//**
//**
//** N.B. :
//**        This function is called from "scilab/scilab.star" at Scilab's startup

  %scicos_display_mode = 0 ; //** obsolete: with the new graphics the display mode
                             //** is fixed

  %scicos_with_grid = %f; //** define grid

  //** %scs_wgrid(1:2) : space of grid
  //** %scs_wgrid(3) : color
  %scs_wgrid = [10;10;12]; //**define aspect of grid

  //Scicos palettes names ============================================================
  //**
  pal_names = ['Sources'    ;'Sinks'           ;'Linear'          ;'Non_linear';
               'Matrix'     ;'Integer'         ;'Events'          ;'Threshold' ;
               'Others'     ;'Branching'       ;'Electrical'      ;'ThermoHydraulics';
               'OldBlocks'  ;'DemoBlocks'];

  scicos_pal= [pal_names  'SCI/macros/scicos/'+pal_names+'.cosf']

  //Scicos palettes libs list ========================================================
  //**
  scicos_pal_libs = ['Branching','Events','Misc','Sinks','Threshold','Linear', ...
	             'MatrixOp','NonLinear','Sources','Electrical','Hydraulics','PDE',...
	             'IntegerOp'];

  //Scicos Menu definitions==========================================================
  //**

  File     = ['File',..
              'New',..
	      'Open',..
	      'Rename',..
	      'Save',..
	      'Save As',..
	      'Save as Interf Func',..
	      'Export',..
	      'Export All',..
              'Exit Scicos',..
	      'Quit' ];

  Diagram  = ['Diagram',..
              'Replot',..
	      'Purge',..
	      'Set Diagram Info',..
	      'Region to Super Block',..
              'Up To Main Diagram',..
	      'Context' ];
  
  Palette  = ['Palette',..
              'Palettes',..
	      'Pal editor',..
	      'Load as Palette',..
	      'Save as Palette',..
              'Pal Tree'];

  Edit     = ['Edit',..
              'Undo',..
	      'Cut',..
	      'Copy',..
	      'Paste',..
	      'Duplicate',..
	      'Delete',..
	      'Move',..
	      'Smart Move',..
	      'Align',..
	      'Flip',..
	      'Rotate Left',..
	      'Rotate Right',..
	      'Add new block',..
	      'Block Documentation'..
	      'Label'];

  View     = ['View',..
              'Zoom in',..
	      'Zoom out',..
              'Fit diagram to figure',..
              'Available Parameters',..
              'Grid'];

  Simulate = ['Simulate',..
              'Setup',..
	      'Compile',..
	      'Eval',..
	      'Debug Level',..
	      'Run'];

  Format   = ['Format',..
              'Set grid',..
   	      'Add color',..
	      'Default link colors',..
	      'Color',..
	      'Background color',..
	      'Show Block Shadow',..
	      'Resize',..
	      'Identification',..
	      'ID fonts',..
	      'Icon',..
	      'Icon Editor'];

  Tools    = ['Tools',..
              'Get Info',..
              'Details',..
              'Browser',..
	      'Code Generation',..
	      'Shortcuts',..
	      'Activate Scilab Window',..
	      'Calc'];

  Help     = ['?',..
              'Help',..
	      'Demos',..
	      'About Scicos'];

  %scicos_menu = list(File,Diagram,Palette,Edit,View,Simulate,Format,Tools,Help);

  //Scicos Right Mouse Button Menu ===========================================
  //**

    //**----------------------------- RIGHT MOUSE BUTTON MENUS (Popup) ------------------------
  //** Right Mouse Button Menus:
  //**        "%scicos_lhb_list" data structure initialization
  //**
  //** Create an empty list() data structure
  %scicos_lhb_list = list();

  //** Fill the data structure with menu/command/functions definitions

  //** state_var = 1 : right click over a valid object inside the CURRENT Scicos Window
  %scicos_lhb_list(1) = list('Open/Set',..
			     'Cut',..
			     'Copy',..
			     'Smart Move',..
			     'Move',..
			     'Duplicate',..
			     'Delete',..
			     'Link',..
			     'Align',..
			     'Flip',..
			     'Rotate Left',..
			     'Rotate Right',..
			     list( 'Properties',..
			           'Resize',..
			           'Icon',..
			           'Icon Editor',..
			           'Color',..
			           'Label',..
			           'Get Info',..
                                   'Details',..
			           'Identification',..
			           'Block Documentation'),..
			     'Code Generation',..
			     'Help');

  //** state_var = 2 : right click in the void of the CURRENT Scicos Window
  %scicos_lhb_list(2) = list('Undo',..
                             'Paste',..
			     'Palettes',..
			     'Context',..
			     'Add new block',..
			     'Region to Super Block',..
			     'Replot',..
			     'Save',..
			     'Save As',..
			     'Load',..
			     'Export',..
			     'Quit',..
			     'Background color',..
			     'Show Block Shadow'  ,..
			     'Zoom in' ,..
			     'Zoom out',..
                             'Pal Tree',..
                             'Browser',..
                             'Details',..
			     'Help');

  //** state_var = 3 : right click over a valid object inside a PALETTE or
  //**                 not a current Scicos window
  //** 
  %scicos_lhb_list(3) = list('Copy',..
			     'Help');
 //**------------------------------

  //Scicos Shortcuts definitions===========================================
  //** single key shortcut "quick menu"
  %scicos_short=['a','Align'
		 'd','Delete';
		 'c','Duplicate';
		 'm','Move';
		 'z','Smart Move';
		 'u','Undo';
		 'f','Flip';
		 't','Rotate Left';
		 'T','Rotate Right';
		 'o','Open/Set';
		 's','Save';
		 'i','Get Info';
		 'r','Replot';
		 'l','Link';
		 'q','Quit';
		 '-','Zoom out';
		 '+','Zoom in';
		 '=','Fit diagram to figure';
		 'g','Grid';
		 'h','Help']

  //Scicos Modelica librabry path definitions==============================
  modelica_libs='SCI/macros/scicos_blocks/'+['Electrical','Hydraulics'];

  //add TMPDIR/Modelica for generic modelica blocks
  status=mkdir(TMPDIR,'Modelica');
  if isdir(TMPDIR+'/Modelica') then 
    modelica_libs=[modelica_libs,TMPDIR+'/Modelica'];
  end

  //** Scicos "xinfo" messages ===========================================
  //**
  //** "%CmenuTypeOneVector" store the list of the commands/function to be called that require both 'Cmenu' AND '%pt'
  //** menus of type 1 (require %pt)
  %CmenuTypeOneVector =..
     ['Region to Super Block', "Press lef mouse button, drag region and release (right button to cancel)";
      'Smart Move',            "Click object to move, drag and click (left to fix, right to cancel)";
      'Move',                  "Click object to move, drag and click (left to fix, right to cancel)";
      'Duplicate',             "Click on the object to duplicate, drag, click (left to copy, right to cancel)";
      'Align',                 "Click on an a port, click on a port of object to be moved";
      'Link',                  "Drag, click left for final or intermediate points or right to cancel";
      'Delete',                "Delete: Click on the object to delete";
      'Flip',                  "Click on block to be flipped"      ;
      'Rotate Left',           "Click on block to be turned left"  ;
      'Rotate Right',          "Click on block to be turned right" ;
      'Open/Set',              "Click to open block or make a link";
      'MoveLink',              ''                                  ; //** hidden commands 
      'SelectLink',            ''                                  ;
      'CtrlSelect',            ''                                  ;
      'SelectRegion',          ''                                  ;
      'Popup',                 ''                                  ;
      'Label',                 "Click block to label";
      'Get Info',              "Click on object  to get information on it";
      'Code Generation',       "Click on a Superblock (without activation output) to obtain a coded block!" ;
      'Icon',                  "Click on block to edit its icon";
      'Color',                 "Click on object to choose its color";
      'Identification',        "Click on an object to set or get identification";
      'Resize',                "Click block to resize";
      'Block Documentation',   "Click on a block to set or get it''s documentation"
     ]

  //**-----------------------------

  //Scicos Help definitions===========================================
  //** 08/09/07 : Alan's patch

  deff('%scicos_help=scicos_help(%scicos_help,entry,doc)',...
       ['ind_entry=find(getfield(1,%scicos_help)==entry)';
        'if ind_entry==[] then '+...
        'setfield(1,[%scicos_help(1),entry],%scicos_help), '+...
        'setfield(lstsize(%scicos_help)+1,doc,%scicos_help), '+...
        'else '+...
        'setfield(ind_entry,doc,%scicos_help), '+...
        'end']);

  //Enter here Menus Help definitions=========================
  %scs_help_menu=tlist(['menu']);

  //****** File Menu ******/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'New',...
               [" Clicking on the New menu item loads an empty diagram in the";
                " active editor Scicos window. If the previous content of the";
                " window is not saved, it will be lost."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Open',...
               [" Select the Open menu item to load an ASCII or binary file";
                " containing a saved block diagram or palette.";
                " A dialog box allows user choosing the file."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Rename',...
               [" This menu allows to change the diagram name. An editable";
                "  dialog box opens."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Save',...
               [" Select the save menu item to save the block diagram";
                " in a binary file already selected by a previous";
                " select the Save As menu item. If you select this";
                " menu item and you have never clicked on the Save As";
                " menu item, the diagram is saved in the current directory";
                " as ""window_name"".cos where ""window_name"" is the name";
                " of the window appearing on top of the window (usually";
                " Untitled or Super Block).";
                " ";
                " The .cos binary files are machine independent."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Save As',...
               [" Select the Save As menu item to save the block diagram or";
                " palette in a file. A dialog box allows choosing the file";
                " which must have a .cos or .cosf extension. The diagram";
                " takes the name of the file (without the extension).";
                " ";
                " If extension is "".cosf"" an ASCII  formatted save is performed";
                " instead of binary save. Formatted save is slower than regular ";
                " save."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Save as Interf Func',...
               [" Select ""the Save as Interf. Func."" menu item to save the ";
                " diagram as a new Scicos block. A Scilab function is generated";
                " and saved in a file with "".sci"" extension. File name and path";
                " are to be set in a ""File menu"" dialog."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Export',...
               [" This menu is used to export a figure of the "
                " current Scicos diagram. The export can be done"
                " directly in postscript format or done first in a"
                " graphic window to export in a second step in all"
                " the format that scilab can provide."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Export All',...
               [" This menu is used to export Scicos diagrams in postscript format."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Exit Scicos',...
               [" Click on the Exit Scicos menu item to close Scicos.";
                " That menu allows to return to Scilab session ";
                " even if the current diagram is a Super block."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Quit',...
               [" Click on the Quit menu item to close current diagram. ";
                " If current diagram is not a Super block, Exit menu closes ";
                " Scicos and returns to Scilab session. Save your diagram ";
                " or/and your palette before leaving.";
                " ";
                " File/Close menu has the same effect."])

  //**** Diagram Menu *****/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Replot',...
               [" Select the Replot menu item to replot the content of";
                " the graphics window. Graphics window stores complete";
                " history of the editing session in memory.";
                " ";
                " Replot is useful for ''cleaning'' this memory."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Purge',...
               [" Select the Purge menu item to get a clean data structure.";
                " If diagram has been hugely modified many deleted blocks";
                " may remain in the data structure. It may be  useful to";
                " suppress then before saving."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Set Diagram Info',...
               [" This menu allows to set user''s diagram information.";
                " This info is stored in the diagram data structure";
                " and may be used for diagram documentation.";
                " ";
                " Information format may be redefined by user."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Region to Super Block',...
               [" This menu allows to transform a rectangular region of the";
                " current diagram by a super block.";
                " Click  on a corner of the region, drag an click left to";
                " fix the region (left click cancels selection).";
                " ";
                " Region is replaced by a super block and links are redrawn."])

//TODO
//   %scs_help_menu=scicos_help(%scs_help_menu,...
//               'Up To Main Diagram',...
//                [""])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Context',...
               [" When you select this menu item you obtain a dialog to enter ";
                " Scilab instructions for defining symbolic Scicos parameters ";
                " used in block definitions or to do whatever you want.";
                " ";
                " These instructions will be evaluated each time the diagram ";
                " is loaded.";
                " ";
                " If you change the value of a symbolic Scicos parameters in ";
                " the context, all the blocks are updated (Eval is performed)."])

  //**** Palette Menu *****/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Palettes',...
               [" Select the Palettes menu item to open a predefined palette."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Pal editor',...
               [" Pal editor allows you to define and reorder the palettes."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Load as Palette',...
               [" Select this Load menu item to load an ASCII or binary file";
                " containing a saved block diagram as a palette.";
                " A dialog box allows user choosing the file."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Save as Palette',...
               [" Select the Save as Palette menu item to save the block diagram"
                " as a palette in a file. A dialog box allows choosing ";
                " the file which must have a .cos or .cosf extension. The palette";
                " takes the name of the file (without the extension).";
                " ";
                " If extension is "".cosf"" an ASCII  formatted save is performed";
                " instead of binary save. It may take a while.";
                " ";
                " .scilab user file is updated if necessary."])
//TODO
//  %scs_help_menu=scicos_help(%scs_help_menu,...
//               'Pal Tree',...
//               [""])

  //****** Edit Menu ******/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Undo',...
               [" Select the Undo menu item to undo the last edit operation.";
                " It is not possible to undo more!."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Cut',...
               [" Cut is used to remove the selected object from the diagram";
                " and keep a copy in the clipboard if the object is a block."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Copy',...
               [" Copy is used to place a copy of the selected object";
                " in the clipboard if the object is a block."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Paste',...
               [" Paste places the object in the Clipboard in the diagram."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Duplicate',...
               [" To duplicate a block in the active editor Scicos window";
                " select first the Duplicate menu item, then";
                " click (with left button) on the to-be-copied block";
                " in Scicos windows or in a palette), and";
                " finally click left where you want the copy";
                " be placed in the active editor Scicos window.";
                " ";
                " The lower left corner of the block is placed";
                " at the selected point.";
                " ";
                " Right click cancel the copy action."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Delete',...
               [" To delete  blocks or a links, select first the Delete";
                " menu item, then click successively on the selected objects";
                " (with left button).";
                " ";
                " When you delete a block all links connected to it";
                " are deleted as well."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Move',...
               [" To move a block in the active editor Scicos window";
                " or in edited palette,";
                " select first the Move menu item, ";
                " then click on the selected block, link segment or link";
                " corner, drag the mouse to the desired new block position ";
                " and click left again to fix the position.";
                " ";
                " Right click cancel the move action."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Smart Move',...
               [" To move a block in  the active editor Scicos window";
                " or in edited palette keeping horizontal and vertical";
                " links, select first the ""Smart Move"" menu item, ";
                " then click on the selected block, link segment or link";
                " corner, drag the mouse to the desired new  position ";
                " and click left again to fix the position.";
                " ";
                " Right click cancel the move action."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Align',...
               [" To obtain nice diagrams, you can align ports of";
                " different blocks, vertically and horizontally.";
                " Select first the Align menu item, then click on the first";
                " port and finally on the second port.";
                " The block corresponding to the second port is moved.";
                " ";
                " A connected block cannot be aligned."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Flip',...
               [" To reverse the positions of the (regular) inputs";
                " and outputs of a block placed on its sides,";
                " select the Flip menu item first and then click on the";
                " selected block. This does not affect the order,";
                " nor the position of the input and output event";
                " ports which are numbered from left to right."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Rotate Left',...
               [" Rotate Left allows to turn a block on the Left.";
                " Each time the block is turned left, his angle is";
                " decresead of 45 degrees.";
                " ";
                " If no blocks or many blocks are selected, this is";
                " the block under the mouse pointer which turns."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Rotate Right',...
               [" Rotate Right allows to turn a block on the right.";
                " Each time the block is turned right, his angle is";
                " incresead of 45 degrees.";
                " ";
                " If no blocks or many blocks are selected, this is";
                " the block under the mouse pointer which turns."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Add new block',...
               [" To add a newly defined block to the current palette or diagram";
                " select first this menu item. A dialog box will pop up ";
                " asking for the name of the GUI function associated ";
                " with the block. If this function is not already loaded";
                " it was search in the current directory. The user may then";
                " click at the desired position of the block icon."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Block Documentation',...
               [" This menu allows to set or get documentation for a block.";
                " ";
                " Selecting this menu and clicking on a block opens an";
                " editable dialog box."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Label',...
               [" To add a label to block, select first this menu item, click next";
                " on the desired block. A dialog appear that allows you to enter ";
                " the desired label.";
                " Labels are used to import data from a block in an other one."])

  //****** View Menu ******/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Zoom in',...
               [" When you select this menu item the diagram is zoomed in ";
                " by a factor of 10%."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Zoom out',...
               [" When you select this menu item the diagram is zoomed out ";
                " by a factor of 10%."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Fit diagram to figure',...
               [" When you select this menu item";
                " the diagram is fit to the size";
                " of the current window."])

//TODO
//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Available Parameters',...
//                [""])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Grid',...
               [" Show/hide a grid in the current diagram."])

  //**** Simulate Menu ****/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Setup',...
               [" In the main Scicos window, clicking on the Setup menu item";
                " invokes a dialog box that allows you to change ";
                " integration parameters: ";
                " ";
                "   - Final integration time (integration ends at";
                "           this time, it always starts from 0), ";
                " ";
                "   - Real time scaling (forces real time simulation";
                "           by setting Scicos unit of time to 1 second), ";
                " ";
                "   - Absolute and relative error tolerances (solver properties), ";
                " ";
                "   - Time tolerance (the smallest time interval for which ";
                "         the ode solver is used to update continuous states), ";
                " ";
                "   - Max integration time interval (the maximum time interval";
                "     for each call to solver, it must be reduced if the error";
                "     message ""too many calls"" is encountered), ";
                " ";
                "   - Solver (choose the numerical solver to be used), ";
                " ";
                "   Max step size (max time step taken by solver)."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Compile',...
               [" Select the Compile menu item to compile the block diagram.";
                " This menu item need never be used since compilation is";
                " performed automatically, if necessary, before";
                " the beginning of every simulation (Run menu item).";
                " ";
                " Normally, a new compilation is not needed if only";
                " system parameters and internal states are modified.";
                " In some cases however these modifications are not";
                " correctly updated and a manual compilation may be";
                " needed before a Restart or a Continue.";
                " Please report if you encounter such a case."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Eval',...
               [" All dialogs user answers may be Scilab instructions.";
                " They are evaluated immediately and stored as character strings.";
                " Select this menu item to have them re-evaluated according to";
                " new values of underlying Scilab variables. ";
                " ";
                " These underlying Scilab variables may be user global variables";
                " defined before Scicos was launch, They may also be defined in";
                " by the Scicos context (see Context menu item)."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Debug Level',...
               [" Set the debug level: 0 (no debugging), 1 (light";
                " debugging information printout), 2 (more information";
                " printout and execution of Debug Block if any in diagram),"
                " 3 (silent debugging mode)."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Run',...
               [" Select the Run menu item to start the simulation.";
                " If the system has already been simulated, a";
                " dialog box appears where you can choose to Continue,";
                " Restart or End the simulation.";
                " ";
                " You may interrupt the simulation by clicking on the ";
                " ""stop"" button, change any of the block parameters";
                " and continue the simulation with the new values."])

  //***** Format Menu *****/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Set grid',...
               [" Set the size and the color of ";
                " the grid for the current diagram."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Add color',...
               [" This menu allows to add new color to the diagram private";
                " color map. Added colors are stored in the diagram data structure."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Default link colors',...
               [" This menu allows to change the default color for regular ";
                " and activation links."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Color',...
               [" To change the background color of an object, select first ";
                " this menu item, click next on the desired object. A dialog appear";
                " that allows you to choose the desired color."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Background color',...
               [" This menu allows to change the background and default";
                " foreground colors."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Show Block Shadow',...
               [" This menu allows to select 3D shape for blocks and ";
                " associated parameters."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Resize',...
               [" To change the size of a block, select first this menu item,";
                " click next on the desired block. A dialog appear that allows ";
                " you to change the width and/or height of the block shape."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'ID fonts',...
               [" This menu allows to change the font used to write in the block";
                " identifications (see ""Set block ID"" menu)."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Identification',...
               [" This menu allows to set an identificator to a link or a block ";
                " block identificators are drawn under the block icon. Super blocks";
                " input/output ports identificators are replicated over the block";
                " shape ports. Links identificators are not displayed.";
                " ";
                " Selecting this menu and clicking on a block or links opens an";
                " editable dialog box."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Icon',...
               [" To change the icon of a block, select first this menu item,";
                " click next on the desired block. A dialog appear that allows ";
                " you to enter Scilab instructions used to draw the icon.";
                " ";
                " You may use the icon_editor menu to generate the Scilab";
                " instructions."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Icon Editor',...
               [" Icon Editor allows you to define graphically the icon of the block."])

  //***** Tools Menu ******/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Get Info',...
               [" This menu allows to get information on an object and on ";
                " its connection with other diagram objects.";
                " ";
                " Select this menu and click on an object."])
//TODO
//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Details',...
//                [""])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Browser',...
               [" This menu opens a graphic window with a tree ";
                " representation of the super blocks hierarchy.";
                " Each node represents a superblock.";
                " ";
                " Browser window is useful to open directly a super-block";
                " everywhere in the hierarchy."]);

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Code Generation',...
               [" This menu allows to generate the simulation code associated with";
                " a discrete time Superblock.";
                "";
                " The code generation is obtained simply by selecting this menu and  ";
                " then the desired superblock.";
                " ";
                " If the superblock satisfies the required conditions, a dialog box";
                " pops up to ask for a block name, a directory where to put the";
                " generated files and for optional libraries requested by the linker.";
                " ";
                " Given this information the code is generated, compiled and linked with ";
                " Scilab. The superblock is automatically replaced by a new block which ";
                " implements the generated code. It is then possible to run the modified ";
                " diagram."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Shortcuts',...
               [" Used to define personalized shortcuts."])

//TODO
//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Activate Scilab Window',...
//                [""])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Calc',...
               [" When you select this menu item you switch Scilab to ";
                " the pause mode (see the help on pause).";
                " In the Scilab main window and you may enter Scilab instructions";
                " to compute whatever you want.";
                " To go back to Scicos you need enter the ""return"" or";
                " ""[...]=return(...)"" Scilab instruction.";
                " ";
                " If you use ""[...]=return(...)"" Scilab instruction take care";
                " not to modify Scicos variables such as ""scs_m"".";
                " ";
                " If you have modified Scicos graphic window you may restore it ";
                " using the Scicos ""Replot"" menu."])

  //****** Help Menu ******/
  //***********************/

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Help',...
               [" To get help on an object or menu buttons,";
                " select first Help menu item and then on ";
                " the selected object or menu item."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'Demos',...
               [" The Demos menu allows to open some";
                " examples of scicos diagram."])

  %scs_help_menu=scicos_help(%scs_help_menu,...
               'About Scicos',...
               ["About Scicos item display the";
                "current version of Scicos and";
                "gives some useful informations."])

//****** Old items ******/
//***********************/
//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Display mode',...
//                ["Display mode can be 0 or 1. In mode 1, scicos editor uses the";
//                 "pixmap option of Scilab graphics to create   a backing";
//                 "store for the graphics used for displaying scicos diagrams. ";
//                 "By default, Display mode is 0. Under Windows operating system";
//                 "the backing store is handled in a different way. "])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Window',...
//                [" In the active editor Scicos window, clicking on the ";
//                 " Window menu item invokes a dialog box that allows you to change ";
//                 " window dimensions."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Focus',...
//                [" Focus menu allows to select a zone (click left, drag zone, click";
//                 " to select) which is focused on.";
//                 " To change focus back use ""Zoom in"" menu."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Shift',...
//                [" To shift the diagram to left, right, up or down,";
//                 " select this menu item, then click on the point you want ";
//                 " to appear in the middle of the graphics window. "])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Duplicate Region',...
//                [" To copy a region in the active editor Scicos window";
//                 " select first the Copy menu item, then";
//                 " click (with right button) on a corner of the desired";
//                 " region (in Scicos windows or in a palette), drag to ";
//                 " define the region, click to fix the region  and";
//                 " finally click left where you want the copy.";
//                 " to be placed in the active editor Scicos window.";
//                 " NOTE: If source diagram is big be patient, region selection ";
//                 " may take a while.";
//                 " ";
//                 " The lower left corner of the block is placed";
//                 " at the selected point.";
//                 " ";
//                 " Right click cancel the copy action."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Replace',...
//                [" To replace a block in the active editor Scicos window";
//                 " select first the Replace menu item, then";
//                 " select the replacement block (in Scicos window or in a";
//                 " palette), and  finally click on the to-be-replaced block."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Link',...
//                [" To connect an output port to an input port,";
//                 " select first the Link menu item, then click on the output";
//                 " port, drag, click left on each intermediate points";
//                 " and finally click left on the input port.";
//                 " ";
//                 " To split a link, select first  the Link menu item,";
//                 " then click left on the link where the split should be placed,";
//                 " drag, click left on each intermediate points";
//                 " and finally click left on the input port.";
//                 " ";
//                 " Right click cancel the link action";
//                 " ";
//                 " Only one link can go from and to a port.";
//                 " Link color can be changed directly by clicking";
//                 " on the link.";
//                 " ";
//                 " This menu remains active until user choose another one."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Delete Region',...
//                ["To delete a blocks in  a region, select first  the Delete Region";
//                 " menu item, then click  on a corner of the ";
//                 " desired region, drag to define the region, and click left to ";
//                 " fix the region. All connected links will be destroyed as";
//                 " well";
//                 " ";
//                 " Right click instead of left cancels the delete action."])

//   %scs_help_menu=scicos_help(%scs_help_menu,...
//                'Open/Set',...
//                [" To change the parameters of a regular block or link, or ";
//                 " to open a super block, select first ";
//                 " this menu item, click next on the desired object.";
//                 " A dialog or an editing window appears";
//                 " which allows you to modify object properties.";
//                 " ";
//                 " It is also possible to select a super block to open clicking";
//                 " on a node of the ""Navigator"" window."])
  //==========================================================

  //Enter here Scilab list Help definitions=========================
  %scs_help_scilst=tlist(['scilst',...
                          'diagr','params','model',...
                          'graphics','link','state','sim']);

  //** define scs_help_scilst lists
  %scs_help_diagr=tlist(['lst']);    //** diagr
  %scs_help_params=tlist(['lst']);   //** params
  %scs_help_model=tlist(['lst']);    //** model
  %scs_help_graphics=tlist(['lst']); //** graphics
  %scs_help_link=tlist(['lst']);     //** link
  %scs_help_state=tlist(['lst']);    //** state
  %scs_help_sim=tlist(['lst']);      //** sim

  //****** scicos_diagram ******/
  //****************************/
  %scs_help_diagr=scicos_help(%scs_help_diagr,...
                 'props',...
                 [" Diagram properties.";
                  " This entry contains various informations such";
                  " some main diagram initials values.";
                  " ";
                  " This variable is a tlist of type {params} and";
                  " contains {wpar}, {title}, {tol}, {tf}, {context},";
                  " {options} and {doc}";
                  " ";
                  " Size : 11";
                  " Type : Scilab tlist of type {params}."])

//   %scs_help_diagr=scicos_help(%scs_help_diagr,...
//                  'props',...
//                  [" Propriétés du diagramme.";
//                   " Cette entrée contient des informations diverses";
//                   " telles que des valeurs initiales du diagramme";
//                   " principal.";
//                   " ";
//                   " Cette variable est une tlist de type {params} et";
//                   " contient {wpar}, {title}, {tol}, {tf}, {context},";
//                   " {options} and {doc}";
//                   " ";
//                   " ";
//                   " Taille 11: ";
//                   " Type : tlist scilab de type {params}."])

  %scs_help_diagr=scicos_help(%scs_help_diagr,...
                 'objs',...
                 [" List of objects included in the Scicos diagram.";
                  " The objects used in scicos are {block}, {link} and Text.";
                  " The objects can also be deleted object data structure.";
                  " Deleted object data structure is marked list(''Deleted'').";
                  " ";
                  " Size : total number of objects in the diagram.";
                  " Type : Scilab tlist of type {block}, {link} or Text."])

//   %scs_help_diagr=scicos_help(%scs_help_diagr,...
//                  'objs',...
//                  [" Une liste d''objets inclus dans le diagramme Scicos.";
//                   " Les objets utilisés dans Scicos sont {block}, {link} et";
//                   " Text.";
//                   " Les objets peuvent aussi être des strucutures de donnée";
//                   " effacées. Les objets effacés sont marqués par list(''Deleted'').";
//                   " ";
//                   " Taille : nombre total d''objets dans le diagramme.";
//                   " Type : tlist scilab de type {block}, {link} ou Text."])

  %scs_help_diagr=scicos_help(%scs_help_diagr,...
                 'version',...
                 [" A string that gives the version of the Scicos diagram.";
                  " This is used to provide compatibility with old diagram."
                  " Note that you can get the current version of Scicos by";
                  " using the entry ''About scicos'' in the help menu"
                  " or by using the function get_scicos_version()."
                  ""
                  " Size : 1";
                  " Type : String"])

//   %scs_help_diagr=scicos_help(%scs_help_diagr,...
//                  'version',...
//                  [" Une chaîne de caractères qui donne le numéro de version";
//                   " du diagramme Scicos. Cela est utilisé pour fournir une";
//                   " compatibilité avec les anciens diagrammes."
//                   " Notez que vous pouvez retrouver la version courante de"
//                   " Scicos en utilisant l''entrée ''About scicos'' dans le"
//                   " menu help ou en utilisant la fonction get_scicos_version()."
//                   " ";
//                   " Taille : 1";
//                   " Type : chaîne de caractères."])


  //****** scicos_params ******/
  //***************************/
  %scs_help_params=scicos_help(%scs_help_params,...
                 'wpar',...
                 [" This vector is not currently used. "
                  " It may be used in the future to code "
                  " window sizes of the editor.";
                  " ";
                  " Size : 6";
                  " Type : column vector or real."])
//  %scs_help_params=scicos_help(%scs_help_params,...
//                 'wpar',...
//                  [" Ce vecteur n''est actuellement pas utilisé.";
//                   " Il devrait être utilisé dans le futur";
//                   " pour enregistrer les paramètres de la"
//                   " fenêtre tels que la position et la taille.";
//                   " ";
//                   " Taille : 6";
//                   " Type : vecteur colonne de nombre réels."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'title',...
                 [" Vector of character strings, where the first one is the"
                  " diagram title and default name of save file name, "
                  " and the second one is the path of the directory of "
                  " the file name.";
                  " ";
                  " Size : 2";
                  " Type : line vector of strings."])
//   %scs_help_params=scicos_help(%scs_help_params,...
//                  'title',...
//                  [" Vecteur de chaîne de caractères, où la première contient";
//                   " le titre du diagramme (le nom par défaut du ficher de";
//                   " sauvegarde) et la deuxième est le chemim du répertoire.";
//                   " ";
//                   " Taille : 2";
//                   " Type : vecteur line de chaîne de caractères."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'tol',...
                 ["";
                  " A vector containing simulation parameters including "
                  " various tolerances used by the solver: ";
                  " ";
                  "  - {atol}   : Integrator absolute tolerance for the numerical solver";
                  " ";
                  "  - {rtol}   : Integrator relative tolerance for the numerical solver.";
                  " ";
                  "  - {ttol}   : Tolerance on time. If an integration period is less";
                  "               than {ttol}, the numerical solver is not called."
                  " ";
                  "  - {deltat} : Maximum integration time interval. If an integration";
                  "               period is larger than {deltat}, the numerical solver"
                  "               is called more than once in such a way that for each call"
                  "               the integration period remains below {deltat}";
                  " ";
                  "  - {scale}  : Real-time scaling; the value 0 corresponds to no"
                  "               real-time scaling. It associates a Scicos simulation";
                  "               time to the real time in seconds. A value of 1 means "
                  "               that each Scicos unit of time corresponds to one second.";
                  " ";
                  "  - {solver} : Choice of numerical solver. The value 0 implies";
                  "               an ODE solver and 100 implies a DAE solver."
                  " ";
                  "  - {hmax}   : Maximum step size for the numerical solver.";
                  "               0 means no limit.";
                  " ";
                  " Size : 7";
                  " Type : column vector of real."])

//   %scs_help_params=scicos_help(%scs_help_params,...
//                  'tol',...
//                  [" Un vecteur contenant les paramètres de la simulation qui";
//                   " inclue les tolérances utilisées par le solveur: ";
//                   " ";
//                   "  - {atol}   : tolérance absolue pour l''intégration";
//                   " ";
//                   "  - {rtol}   : tolérance relative pour l''intégration.";
//                   " ";
//                   "  - {ttol}   : tolérance sur le temps. Si une période d''intégration est";
//                   "               inférieure à {ttol}, le solveur n''est pas appellé."
//                   " ";
//                   "  - {deltat} : Interval temporel maximal entre deux points d''intégration."
//                   "               Si une période d''intégration est plus grande que {deltat}, ";
//                   "               le solveur est appelé plus d''une fois de manière à maintenir"
//                   "               le pas au niveau de {deltat}";
//                   " ";
//                   "  - {scale}  : Facteur d''échelle temps-réel (Real-time scaling);"
//                   "               la valeur 0 correspond à un facteur d''échelle nul."
//                   "               Ce facteur associe le temps de la simulation au temps réel en"
//                   "               seconde. Une valeur égale à 1, signifie que chaque untité de temps"
//                   "               Scicos correspond à une seconde."
//                   " ";
//                   "  - {solver} : Choix du solveur numérique. Une valeur 0 signifie qu''un solveur ODE"
//                   "               sera utilisé et 100 signifie qu''un solveur DAE sera utilisé."
//                   " ";
//                   "  - {hmax}   : Pas maximal d''intégration pour le solveur.";
//                   "               0 signifie pas de limites.";
//                   " ";
//                   " Taille : 7";
//                   " Type : vecteur colonne de nombre réels."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'tf',...
                 [" Final time simulation. The simulation stops at this";
                  " time. The default value is 100000!";
                  " ";
                  " Size : 1";
                  " Type : real."])
//    %scs_help_params=scicos_help(%scs_help_params,...
//                   'tf',...
//                  [" Le temps final de simulation. La simulation s''arrête";
//                   " a ce temps. La valeur par défaut est 100000!";
//                   " ";
//                   " Taille : 1";
//                   " Type : réel."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'context',...
                 [" A vector of strings containing Scilab instructions"
                  " defining Scilab variables to be used inside block GUIs"
                  " as symbolic parameters. All valid Scilab instructions"
                  " can be used and also comments.";
                  " ";
                  " Size : number of lines of the context";
                  " Type : column vector of strings."])
//   %scs_help_params=scicos_help(%scs_help_params,...
//                  'context',...
//                  [" Un vecteur de chaînes de caractères contenant des";
//                   " instructions Scilab définissant des variables";
//                   " Scilab à utiliser dans les boîtes de dialogues ";
//                   " des blocs (comme paramètres symboliques).";
//                   " Toutes instructions valides Scilab peuvent être";
//                   " utilisées ainsi que des commentaires.";
//                   " ";
//                   " Taille : nombre de lignes du contexte.";
//                   " Type : Vecteur colonne de chaînes de caractères."])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'void1',...
                 [" unused field.";
                  " ";
                  " Size : -";
                  " Type : -"])
//   %scs_help_params=scicos_help(%scs_help_params,...
//                  'void1',...
//                  [" Champ inutilisé.";
//                   " ";
//                   " Taille : -";
//                   " Type : -"])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'options',...
                 [" Scilab object of type {scsopt} defining"
                  " graphical properties of the editor such"
                  " as background color and link color."
                  " ";
                  " The fields are the following:";
                  " ";
                  "  - {3D}         : A list with two entries. The first one is a boolean"
                  "                   indicating whether or not blocks should have 3D aspect."
                  "                   The second entry indicates the color in the current colormap"
                  "                   to be used to create the 3D effect. The default is 33"
                  "                   which corresponds to gray added by Scicos to the standard"
                  "                   colormap, which contains 32 colors. The default value is"
                  "                   list(%t,33)."
                  " ";
                  "  - {Background} : Vector with two entries: background and"
                  "                   foreground colors. The default value is"
                  "                   [8,1]."
                  " ";
                  "  - {Link}       : Default link colors for regular and activation"
                  "                   links. These colors are used only at link construction."
                  "                   Changing them does not affect already constructed links."
                  "                   The default value is [1,5], which corresponds to black and"
                  "                   red if the standard Scilab colormap is used."
                  " ";
                  "  - {ID}         : A list of two vectors including font number and"
                  "                   sizes. The default value is [5,1],[4,1]."
                  " ";
                  "  - {Cmap}       : An $n\times 3$ matrix containing RGB values of colors"
                  "                   to be added to the colormap. The default value is [0.8,0.8,0.8],"
                  "                   i.e., the color gray."
                  " ";
                  " Size : 6";
                  " Type : scilab tlist of type {scsopt}"])
//   %scs_help_params=scicos_help(%scs_help_params,...
//                  'options',...
//                  [" Liste Scilab de type {scsopt} définissant les";
//                   " propriétés graphiques de l''éditeur telles que";
//                   " la couleur de fond et la couleur des liens.";
//                   " ";
//                   " Les champs sont les suivant:";
//                   " ";
//                   "  - {3D}         : Une liste à deux entrées. La première est un booléen"
//                   "                   indiquant si les blocs doivent avoir un aspect 3D.";
//                   "                   La seconde entrée indique la couleur dans la palette de"
//                   "                   couleur courante (colormap) qui doit être utilisée pour"
//                   "                   créer l''effet 3D. Par défault celle ci est 33 ce qui"
//                   "                   correspond à gris, couleur ajoutée par Scicos à la palette"
//                   "                   standard qui contients 32 couleurs."
//                   " ";
//                   "                   La valeur par défault de cette list est donc list(%t,33)."
//                   "  - {Background} : Un vecteur avec deux entrées : la couleur d''arrière plan et"
//                   "                   la couleur d''avant plan. La valeur par défaut est [8,1].";
//                   " ";
//                   "  - {Link}       : Les couleurs par défauts pour les liens réguleurs et les"
//                   "                   liens d''activations. Ces couleurs sont utilisées uniquement";
//                   "                   pour de nouvelles constructions. Ces choix n''affectent pas";
//                   "                   les liens déjà présent sur le diagramme.";
//                   " ";
//                   "                   Les valeurs par défaut sont [1,5] ce qui correspond à noir";
//                   "                   et rouge pour la palette de couleurs standard de Scilab.";
//                   "  - {ID}         : Une liste de deux vecteurs qui définit le type de la police"
//                   "                   et la taille de la police. Les valeurs par défaut sont";
//                   "                   [5,1],[4,1].";
//                   " ";
//                   "  - {Cmap}       : Une matrice de taille 3,n contenant les valeurs RGB des"
//                   "                   couleurs à ajouter à la palette de couleur. La valeur par";
//                   "                   défaut est [0.8,0.8,0.8] (couleur grise).";
//                   " ";
//                   " Taille : 6";
//                   " Type : tlist scilab de type {scsopt}"])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'void2',...
                 [" unused field.";
                  " ";
                  " Size : -";
                  " Type : -"])
//   %scs_help_params=scicos_help(%scs_help_params,...
//                  'void2',...
//                  [" Champ inutilisé.";
//                   " ";
//                   " Taille : -";
//                   " Type : -"])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'void3',...
                 [" unused field.";
                  " ";
                  " Size : -";
                  " Type : -"])
//   %scs_help_params=scicos_help(%scs_help_params,...
//                  'void3',...
//                  [" Champ inutilisé.";
//                   " ";
//                   " Taille : -";
//                   " Type : -"])

  %scs_help_params=scicos_help(%scs_help_params,...
                 'doc',...
                 [" User defined diagram documentation structure.";
                  " ";
                  " Size : 1";
                  " Type : Strings."])
//   %scs_help_params=scicos_help(%scs_help_params,...
//                  'doc',...
//                  [" Structure utilisateur pour la documentation des diagrammes.";
//                   " ";
//                   " Taille : 1";
//                   " Type : Chaînes de caractères."])

  //****** scicos_model ******/
  //**************************/
  %scs_help_model=scicos_help(%scs_help_model,...
                 'sim',...
                 ["A list containing two elements. The first element is"
                  "a string containing the name of the computational function"
                  "(C, Fortran,or Scilab). The second element is an integer"
                  "specifying the type of the computational function. Currently"
                  "type 4 and 5 are used, but older types continue to work to "
                  "ensure backward compatibility.";
                  "For some older case, {sim} can be a single string and that"
                  "means that the type is supposed to be 0."
                  " ";
                  " Size : 2";
                  " Type : Scilab list."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'sim',...
//                  [" Une liste Scilab contenant deux éléments. Le premier élément";
//                   " est une chaîne de caractères contenant le nom de la fonction";
//                   " de calcul (C, Fortran ou Scilab). Le deuxième élément est un";
//                   " entier spécifiant le type de la fonction de calcul. Les types";
//                   " courant sont 4 et 5, mais les anciens types sont toujours";
//                   " compatibles.";
//                   " Pour des anciens blocs, {sim} peut être une simple liste, ce";
//                   " qui signifie que le type est supposé être 0.";
//                   " ";
//                   " Taille : 2";
//                   " Type : liste Scilab."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'in',...
                 ["";
                  " A vector specifying the number and size of the first"
                  " dimension of regular input ports indexed from top to";
                  " bottom of the block. If no input port exist in==[].";
                  " ";
                  " Size : number of regular input ports.";
                  " Type : column vector of integer numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'in',...
//                  [" Un vecteur spécifiant le nombre et la première";
//                   " dimension des ports d''entrée réguliers du bloc";
//                   " indéxés du haut vers le bas.";
//                   " Si il n''existe pas de port d''entrées, alors";
//                   " in=[].";
//                   ""
//                   " Taille : nombre de ports réguliers d''entrée.";
//                   " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'in2',...
                 [" A vector specifying the second dimension of regular"
                  " input ports indexed from top to bottom of the block."
                  " {in} with {in2} formed then the regular input sizes matrix.";
                  " For compatibility, this dimension can stay empty ([]). That";
                  " means that the dimensions of input ports will be in,1";
                  " ";
                  " Size : number of regular input ports.";
                  " Type : column vector of integer numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'in2',...
//                  [" Un vecteur spécifiant la deuxième dimension";
//                   " des ports réguliers d''entrées du bloc (indéxé";
//                   " du haut vers le bas)."
//                   " {in} avec {in2} forment alors les tailles des matrices"
//                   " d''entrée."
//                   " Pour des raisons de compatibilité, cette dimension"
//                   " peut rester non renseignée ([]), ce qui signifiera"
//                   " que les dimensions des ports seront in,1."
//                   ""
//                   " Taille : nombre de ports réguliers d''entrée.";
//                   " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'intyp',...
                 [" A vector specifying the types of regular input ports."
                  " Its sizes is equal to the sizes of {in}.";
                  " The types of regular input ports can be :";
                  "  - 1 : real matrix";
                  "  - 2 : complex matrix";
                  "  - 3 : int32 matrix";
                  "  - 4 : int16 matrix";
                  "  - 5 : int8 matrix";
                  "  - 6 : uint32 matrix";
                  "  - 7 : uint16 matrix";
                  "  - 8 : uint8 matrix";
                  " ";
                  " Size : number of regular input ports.";
                  " Type : column vector of integer numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'intyp',...
//                  [" Un vecteur spécifiant les types des ports d''entrée réguliers.";
//                   " Sa taille est égale à {in}. ";
//                   " Les types des ports d''entrée peuvent être :";
//                   "  - 1 : matrice de nombres réels";
//                   "  - 2 : matrice de nombres complexes";
//                   "  - 3 : matrice de int32";
//                   "  - 4 : matrice de int16";
//                   "  - 5 : matrice de int8";
//                   "  - 6 : matrice de uint32";
//                   "  - 7 : matrice de uint16";
//                   "  - 8 : matrice de uint8";
//                   " ";
//                   " Taille : nombre de ports réguliers d''entrée.";
//                   " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'out',...
                 ["";
                  " A vector specifying the number and size of the first"
                  " dimension of regular output ports indexed from top to";
                  " bottom of the block. If no output port exist out==[].";
                  " ";
                  " Size : number of regular output ports.";
                  " Type : column vector of integer numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'out',...
//                  [" Un vecteur spécifiant le nombre et la première";
//                   " dimension des ports de sortie réguliers du bloc";
//                   " indéxés du haut vers le bas.";
//                   " Si il n''existe pas de port de sorties, alors";
//                   " out=[].";
//                   ""
//                   " Taille : nombre de ports réguliers de sortie.";
//                   " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'out2',...
                 [" A vector specifying the second dimension of regular"
                  " output ports indexed from top to bottom of the block."
                  " {out} with {out2} formed then the regular output sizes matrix.";
                  " For compatibility, this dimension can stay empty ([]). That";
                  " means that the dimensions of output ports will be out,1";
                  " ";
                  " Size : number of regular output ports.";
                  " Type : column vector of integer numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'out2',...
//                  [" Un vecteur spécifiant la deuxième dimension";
//                   " des ports réguliers de sortie du bloc (indéxé";
//                   " du haut vers le bas)."
//                   " {out} avec {out2} forment alors les tailles des matrices"
//                   " de sorties."
//                   " Pour des raisons de compatibilité, cette dimension"
//                   " peut rester non renseignée ([]), ce qui signifiera"
//                   " que les dimensions des ports seront out,1."
//                   ""
//                   " Taille : nombre de ports réguliers de sortie.";
//                   " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'outtyp',...
                 [" A vector specifying the types of regular output ports."
                  " Its sizes is equal to the sizes of {out}.";
                  " The types of regular output ports can be :";
                  "  - 1 : real matrix";
                  "  - 2 : complex matrix";
                  "  - 3 : int32 matrix";
                  "  - 4 : int16 matrix";
                  "  - 5 : int8 matrix";
                  "  - 6 : uint32 matrix";
                  "  - 7 : uint16 matrix";
                  "  - 8 : uint8 matrix";
                  " ";
                  " Size : number of regular output ports.";
                  " Type : column vector of integer numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'outtyp',...
//                  [" Un vecteur spécifiant les types des ports de sortie réguliers.";
//                   " Sa taille est égale à {out}. ";
//                   " Les types des ports de sortie peuvent être :";
//                   "  - 1 : matrice de nombres réels";
//                   "  - 2 : matrice de nombres complexes";
//                   "  - 3 : matrice de int32";
//                   "  - 4 : matrice de int16";
//                   "  - 5 : matrice de int8";
//                   "  - 6 : matrice de uint32";
//                   "  - 7 : matrice de uint16";
//                   "  - 8 : matrice de uint8";
//                   " ";
//                   " Taille : nombre de ports réguliers de sortie.";
//                   " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'evtin',...
                 [" A vector specifying the number and sizes of"
                  " activation inputs. Currently activation ports"
                  " can be only of size one.";
                  " If no event input port exists evtin must be "
                  " equal to [].";
                  " ";
                  " Size : number of input event ports.";
                  " Type : column vector of integer numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'evtin',...
//                  [" Un vecteur indiquant la taille et le nombre";
//                   " de ports événementiels d''entrée. Actuellement";
//                   " les ports peuvent seulement avoir une taille";
//                   " égale à 1.";
//                   " Si aucun ports événementiels existe alors";
//                   " evtin est égal à 1.";
//                   " ";
//                   " Taille : nombre de ports événementiel d''entrée.";
//                   " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'evtout',...
                 [" A vector specifying the number and sizes of"
                  " activation outputs. Currently activation ports"
                  " can be only of size one.";
                  " If no event output port exists evtout must be "
                  " equal to [].";
                  " ";
                  " Size : number of output event ports.";
                  " Type : column vector of integer numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'evtout',...
//                  [" Un vecteur indiquant la taille et le nombre";
//                   " de ports événementiels de sorties. Actuellement";
//                   " les ports peuvent seulement avoir une taille";
//                   " égale à 1.";
//                   " Si aucun ports événementiels existe alors";
//                   " evtout est égal à 1.";
//                   " ";
//                   " Taille : nombre de ports événementiel de sortie.";
//                   " Type : vecteur colonne de nombres entiers."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'state',...
                 [" Vector containing initial values of continuous-time state.";
                  " Must be [] if no continuous state.";
                  " ";
                  " Size : number of continuous-time state.";
                  " Type : column vector of real numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'state',...
//                  [" Un vecteur contenant les valeurs initiales des états";
//                   " continus.";
//                   " Ce vecteur doit être égal à [] sie le bloc ne possède";
//                   " pas d''état continus";
//                   " ";
//                   " Taille : nombre d''états continus";
//                   " Type : vecteur colonne de nombres réels"])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'dstate',...
                 [" Vector containing initial values of discrete-time state.";
                  " Must be [] if no discrete state.";
                  " ";
                  " Size : number of discrete-time state.";
                  " Type : column vector of real numbers."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'state',...
//                  [" Un vecteur contenant les valeurs initiales des états";
//                   " discrets.";
//                   " Ce vecteur doit être égal à [] si le bloc ne possède";
//                   " pas d''état discrets";
//                   " ";
//                   " Taille : nombre d''états discrets.";
//                   " Type : vecteur colonne de nombres discrets."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'odstate',...
                 [" List containing initial values of objects state.";
                  " Must be list() if no objects state.";
                  " ";
                  " Size : number of objetcs state.";
                  " Type : scilab list of scilab objects."])
//   %scs_help_model=scicos_help(%scs_help_model,...
//                  'state',...
//                  [" Une liste contenant les valeurs initiales des états";
//                   " objets.";
//                   " Ce vecteur doit être égal à [] si le bloc ne possède";
//                   " pas d''état discrets";
//                   " ";
//                   " Taille : nombre d''états objets.";
//                   " Type : list scilab ."])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'rpar',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'ipar',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'opar',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'blocktype',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'firing',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'dep_ut',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'label',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'nzcross',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'nmode',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_model=scicos_help(%scs_help_model,...
                 'equations',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  //****** scicos_graphics ******/
  //*****************************/
  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'orig',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'sz',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'flip',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'theta',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'exprs',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'pin',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'pout',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'pein',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'peout',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'gr_i',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'id',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'in_implicit',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_graphics=scicos_help(%scs_help_graphics,...
                 'out_implicit',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  //****** scicos_link ******/
  //*************************/
  %scs_help_link=scicos_help(%scs_help_link,...
                 'xx',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'yy',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'id',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'thick',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'ct',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'from',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_link=scicos_help(%scs_help_link,...
                 'to',...
                 ["";
                  " ";
                  " Size : ";
                  " Type : "])
//                  ["";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  //****** scicos_state ******/
  //**************************/
  %scs_help_state=scicos_help(%scs_help_state,...
                 'state',...
                 [" Scilab typed list of type {xcs}. It contains all"
                  " the states of the model, that is, everything than"
                  " can evolve during the simulation.";
                  " ";
                  " state contains {x}, {z}, {oz}, {iz}, "
                  " {tevts}, {evtspt}, {pointi} and {outtb}."
                  " ";
                  " Size : 9.";
                  " Type : scilab tlist"])

//  %scs_help_state=scicos_help(%scs_help_state,...
//                 'state',...
//                  [" Une liste typée scilab de type {xcs}. Cette";
//                   " liste contient tous les états du diagramme qui";
//                   " vont évoluer pendant la simulation";
//                   " ";
//                   " state contient {x}, {z}, {oz}, {iz}, "
//                   " {tevts}, {evtspt}, {pointi} et {outtb}."
//                   " ";
//                   " Taille : 9.";
//                   " Type : tlist scilab."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'x',...
                 [" The continuous-time state register, which is obtained "
                  " by concatenating the continuous-time states of all "
                  " the blocks.";
                  " ";
                  " Size : total of all the size of continuous-time state registers.";
                  " Type : column vector of real numbers."])

//   %scs_help_state=scicos_help(%scs_help_state,...
//                  'x',...
//                  [" Le registre des états continus qui est obtenu";
//                   " en concatenant tous les états continus de tous";
//                   " les blocs."
//                   " ";
//                   " Taille : total des tailles de tous les registres des états continus.";
//                   " Type : vecteur colonne de nombres réels."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'z',...
                 [" The discrete-time state register, which is obtained"
                  " by concatenating the discrete-time states of all"
                  " the blocks.";
                  " ";
                  " Size : total of all the size of discrete-time state registers.";
                  " Type : column vector of real number."])

//   %scs_help_state=scicos_help(%scs_help_state,...
//                  'z',...
//                  [" Le registre des états discrets qui est obtenu";
//                   " en concatenant tous les états discrets de tous";
//                   " les blocs."
//                   " ";
//                   " Taille : total des tailles de tous les registres des états discrets.";
//                   " Type : vecteur colonne de nombres réels."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'oz',...
                 [" The list of the object discrete-time state, which"
                  " is obtained by concatenating the object "
                  " discrete-time states of all the blocks.";
                  " ";
                  " Size : total of all the size of object state.";
                  " Type : list of scilab object."])

//   %scs_help_state=scicos_help(%scs_help_state,...
//                  'oz',...
//                  [" La liste des états objets discrets obtenue";
//                   " par la concaténation de tous les états objets";
//                   " de tous les blocs";
//                   " ";
//                   " Taille : total des tailles de toutes les listes des états objet.";
//                   " Type : liste scilab."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'iz',...
                 [" Vector of size equal to the number of blocks."
                  " That vector is used to store pointers of the "
                  " working state register (work). If a block needs "
                  " to allocate memory at initialization (flag 4), "
                  " the associated pointer is saved here.";
                  " ";
                  " Size : number of blocks.";
                  " Type : column vector of real numbers."])

//   %scs_help_state=scicos_help(%scs_help_state,...
//                  'iz',...
//                  [" Vecteur de taille égale au nombre de blocs.";
//                   " Ce vecteur est utilisé pour enregistrer les"
//                   " pointeurs des registres de travail (work)"
//                   " Si un bloc fait une allocation du registre work"
//                   " à l''initialisation (flag=4) alors le pointeur"
//                   " associé est enregistré ici."
//                   " ";
//                   " Taille : nombre de blocs.";
//                   " Type : vecteur colonne de nombres réels."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'tevts',...
                 [" Vector of size equal to the number of activation"
                  " sources. It contains the scheduled times for"
                  " programmed activations in {evtspt}.";
                  " ";
                  " Size : number of activation sources.";
                  " Type : column vector of integers."])

//   %scs_help_state=scicos_help(%scs_help_state,...
//                  'tevts',...
//                  [" Un vecteur de taille égale au nombre";
//                   " de sources d''activation. C''est une table
//                   " contenant les dates des événements "
//                   " programmés dans {evtspt}."
//                   " ";
//                   " Taille : nombre de sources d''activation.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'evtspt',...
                 [" Vector of size equal to the number of activation"
                  " sources. It is an event scheduler. ";
                  " ";
                  " Size : number of activation sources.";
                  " Type : column vector of integers."])

//   %scs_help_state=scicos_help(%scs_help_state,...
//                  'evtspt',...
//                  [" Un vecteur de taille égale au nombre";
//                   " de sources d''activation. C''est une table
//                   " d''événements."
//                   " ";
//                   " Taille : nombre de sources d''activation.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'pointi',...
                 [" The number of the next programmed event.";
                  " ";
                  " Size : 1.";
                  " Type : integer."])

//   %scs_help_state=scicos_help(%scs_help_state,...
//                  'pointi',...
//                  [" Le numéro du prochain événement";
//                   " ";
//                   " Taille : 1";
//                   " Type : entier."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'outtb',...
                 [" Scilab list containing all output registers "
                  " of blocks. Each element of that list contains "
                  " typed matrix-based data.";
                  " ";
                  " Size : number of regular output ports.";
                  " Type : list of scilab matrix."])

//   %scs_help_state=scicos_help(%scs_help_state,...
//                  'outtb',...
//                  [" Liste scilab contenant tous les registres";
//                   " de sortie des blocs. Chaque éléments de cette"
//                   " liste contient des données typées matricielles."
//                   " ";
//                   " Taille : nombre de ports réguliers de sortie.";
//                   " Type : liste scilab contenant des matrices."])

  //****** scicos_sim ******/
  //************************/
  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'sim',...
                 [" Scilab typed list of type {scs}. It contains "
                  " static arrays coming from the result of the"
                  " compilation. That arrays does not evolve"
                  " during the simulation."
                  " ";
                  " Size : 33";
                  " Type : scilab tlist"])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'sim',...
//                  [" Une liste typée scilab de type {scs}. Cette";
//                   " liste contient des tableaux statiques issus"
//                   " du résultat de la compilation. Ces tableaux"
//                   " n''évoluent pas pendant la simulation."
//                   " ";
//                   " Taille : 33";
//                   " Type : tlist scilab."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'funs',...
                 [" A list containing names of the computational"
                  " functions or scilab functions.";
                  " "
                  " Size : number of blocks.";
                  " Type : list of strings and/or scilab function."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'funs',...
//                  [" Une liste qui contient les noms des";
//                   " fonctions de calculs ou des fonctions";
//                   " scilab";
//                   " ";
//                   " Taille : nombre de blocs.";
//                   " Type : liste scilab."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'xptr',...
                 [" A vector pointer to the continuous time state "
                  " register {x}. The continuous-time state of "
                  " block {i} is "
                  " {state.x(sim.xptr(i):sim.xptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'xptr',...
//                  [" Un vecteur pointant vers le registre des états";
//                   " continus {x}. Le registre des états continus";
//                   " du bloc {i} peut-être retrouvé par :";
//                   " {state.x(sim.xptr(i):sim.xptr(i+1)-1)}.";
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zptr',...
                 [" A vector pointer to the discrete time state "
                  " register {z}. The discrete-time state of "
                  " block {i} is "
                  " {state.z(sim.zptr(i):sim.zptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'zptr',...
//                  [" Un vecteur pointant vers le registre des états";
//                   " discret {z}. Le registre des états discrets";
//                   " du bloc {i} peut-être retrouvé par :";
//                   " {state.z(sim.zptr(i):sim.zptr(i+1)-1)}.";
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ozptr',...
                 [" A vector pointer to the object discrete state "
                  " register {oz}. The object discrete state of "
                  " block {i} is "
                  " {state.oz(sim.ozptr(i):sim.ozptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'ozptr',...
//                  [" Un vecteur pointant vers le registre des états";
//                   " objets discret {oz}. Le registre des états objets";
//                   " du bloc {i} peut-être retrouvé par :";
//                   " {state.oz(sim.ozptr(i):sim.ozptr(i+1)-1)}.";
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zcptr',...
                 [" A vector pointer to the zero-crossing surfaces.";
                  " register. That vector gives by block the used";
                  " number of the zero-crossing.";
                  "";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'zcptr',...
//                  [" Un vecteur pointant vers le registre des";
//                   " détections de surfaces. Ce vecteur indique";
//                   " par bloc le nombre de passages à zéro utilisé.";
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'inpptr',...
                 [" (sim.inpptr(i+1)-sim.inpptr(i)) gives the number of";
                  " regular input ports of the i block.";
                  " inpptr(i) points to the beginning of ith block";
                  " inputs within the indirection table {inplnk}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'inpptr',...
//                  [" (sim.inpptr(i+1)-sim.inpptr(i)) donne le nombre";
//                   " de ports réguliers d''entrée du bloc i.";
//                   " inpptr(i) pointe vers le début des ports d''entrée"
//                   " du bloc i et les connections à ces ports peuvent"
//                   " retrouvées par la table d''indirection {inplnk}."
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'outptr',...
                 [" (sim.outptr(i+1)-sim.outptr(i)) gives the number of";
                  " regular ouyput ports of the i block.";
                  " outptr(i) points to the beginning of ith block";
                  " outputs within the indirection table {outlnk}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'outptr',...
//                  [" (sim.outptr(i+1)-sim.outptr(i)) donne le nombre";
//                   " de ports réguliers de sortie du bloc i.";
//                   " outptr(i) pointe vers le début des ports de sortie"
//                   " du bloc i et les connections à ces ports peuvent"
//                   " retrouvées par la table d''indirection {outlnk}."
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'inplnk',...
                 [" (cpr.sim.inplnk(cpr.sim.inpptr(i)-1+j))"
                  " is the index of the link connected to "
                  " the jth input port of the ith block where "
                  " j goes from 1 to "
                  " (cpr.sim.inpptr(i+1)-cpr.sim.inpptr(i)). ";
                  " ";
                  " Size : total number of regular input port.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'inplnk',...
//                  [" (cpr.sim.inplnk(cpr.sim.inpptr(i)-1+j))";
//                   " est l''index du lien connecté à l''entrée j";
//                   " du bloc i, où j va de 1 jusqu''à";
//                   " (cpr.sim.inpptr(i+1)-cpr.sim.inpptr(i))";
//                   " ";
//                   " Taille : nombre total de port réguliers d''entrée.";
//                   " Type : vecteur colonne de nombre entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'outlnk',...
                 [" (cpr.sim.outlnk(cpr.sim.outptr(i)-1+j))"
                  " is the index of the link connected to "
                  " the jth output port of the ith block where "
                  " j goes from 1 to "
                  " (cpr.sim.outptr(i+1)-cpr.sim.outptr(i)). ";
                  " ";
                  " Size : total number of regular output port.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'outlnk',...
//                  [" (cpr.sim.outlnk(cpr.sim.outptr(i)-1+j))";
//                   " est l''index du lien connecté à la sortie j";
//                   " du bloc i, où j va de 1 jusqu''à";
//                   " (cpr.sim.outptr(i+1)-cpr.sim.outptr(i))";
//                   " ";
//                   " Taille : nombre total de port réguliers de sortie.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'rpar',...
                 [" Vector of real parameters that is "
                  " obtained by concatenating the real" 
                  " parameters registers of all the blocks.";
                  " ";
                  " Size : total number of real parameters.";
                  " Type : column vector of real numbers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'rpar',...
//                  [" Vecteur des paramètres réels qui est";
//                   " obtenu en concatenant les registres"
//                   " des paramètres réels de tous les blocs."
//                   " ";
//                   " Taille : nombre total de paramètres réels.";
//                   " Type : vecteur colonne de nombre réels."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'rpptr',...
                 [" A vector pointer to the real parameters"
                  " register {rpar}. The real parameters of"
                  " block i are "
                  " {sim.rpar(sim.rpptr(i):sim.rpptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integer."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'rpptr',...
//                  [" Un vecteur pointant vers le registre des";
//                   " paramètres réels {rpar}. Les paramètres "
//                   " réels du bloc i peuvent être retrouvés par :";
//                   " {sim.rpar(sim.rpptr(i):sim.rpptr(i+1)-1)}.";
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ipar',...
                 [" Vector of integer parameters that is "
                  " obtained by concatenating the integer"
                  " parameters registers of all the blocks.";
                  " ";
                  " Size : total number of integer parameters.";
                  " Type : column vector of integer."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'ipar',...
//                  [" Vecteur des paramètres entiers qui est";
//                   " obtenu en concatenant les registres"
//                   " des paramètres entiers de tous les blocs."
//                   " ";
//                   " Taille : nombre total de paramètres entiers.";
//                   " Type : vecteur colonne de nombre réels."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ipptr',...
                 [" A vector pointer to the integer parameters"
                  " register {ipar}. The integer parameters of"
                  " block i are "
                  " {sim.ipar(sim.ipptr(i):sim.ipptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of real numbers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'ipptr',...
//                  [" Un vecteur pointant vers le registre des";
//                   " paramètres entier {ipar}. Les paramètres "
//                   " entiers du bloc i peuvent être retrouvés par :";
//                   " {sim.ipar(sim.ipptr(i):sim.ipptr(i+1)-1)}.";
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'opar',...
                 [" List of object parameters that is "
                  " obtained by concatenating the list"
                  " of object parameters of all the blocks.";
                  " ";
                  " Size : total number of object parameters.";
                  " Type : list of scilab objects."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'opar',...
//                  [" Liste des paramètres objets qui est";
//                   " obtenue en concatenant les listes"
//                   " des paramètres objets de tous les blocs."
//                   " ";
//                   " Taille : nombre total de paramètres objets.";
//                   " Type : liste scilab."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'opptr',...
                 [" A vector pointer to the object parameters"
                  " list {opar}. The object parameters of"
                  " block i are "
                  " {sim.opar(sim.opptr(i):sim.opptr(i+1)-1)}.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'opptr',...
//                  [" Un vecteur pointant vers la liste des";
//                   " paramètres objets {opar}. Les paramètres "
//                   " entiers du bloc i peuvent être retrouvés par :";
//                   " {sim.opar(sim.opptr(i):sim.opptr(i+1)-1)}.";
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'clkptr',...
                 [" A vector pointer to output activation ports.";
                  " (cpr.sim.clkptr(i):cpr.sim.clkptr(i+1)-1) "
                  " gives the number of output event ports";
                  " of the block i.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'clkptr',...
//                  [" Un vecteur pointant vers les ports ";
//                   " d''activation de sortie."
//                   " (cpr.sim.clkptr(i):cpr.sim.clkptr(i+1)-1)"
//                   " donne les numéros des ports de sortie"
//                   " événementiel du bloc i."
//                   " "
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ordptr',...
                 [" A vector pointer to {ordclk} designating the"
                  " part of {ordclk} corresponding to a given"
                  " activation.";
                  "(cpr.sim.ordptr(i):cpr.sim.ordptr(i+1)-1)"
                  " points to the region within ordclk indicates "
                  " the number of blocks activated by the output"
                  " event ports numbered i."
                  " ";
                  " Size : number of sources of activation + 1.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'ordptr',...
//                  [" Un vecteur pointant vers {ordclk}";
//                   " (cpr.sim.ordptr(i):cpr.sim.ordptr(i+1)-1)"
//                   " pointe vers la zone de ordclk qui"
//                   " indique les numéros des blocs activés par"
//                   " le port événementiel numérotés i."
//                   " ";
//                   " Taille : nombre de source d''activations + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'execlk',...
                 ["Unused.";
                  " ";
                  " Size : -";
                  " Type : matrix of real."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'execlk',...
//                  ["Non utilisé.";
//                   " ";
//                   " Taille : ";
//                   " Type : "])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ordclk',...
                 [" A matrix associated to blocks activated by"
                  " output activation ports. The first column"
                  " contains the block number, and  the second,"
                  " the event code by which the block should be"
                  " called.";
                  " ";
                  " Size : total number of blocks summed by source of activations.";
                  " Type : matrix of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'ordclk',...
//                  [" Une matrice associé aux blocs activés par";
//                   " événements. La première colonne contient";
//                   " les numéros des blocs et la deuxième le"
//                   " code de l''événement par lequel le bloc"
//                   " sera appelé pendant la simulation"
//                   " ";
//                   " Taille : nombre total de bloc sommé par source d''activations.";
//                   " Type : matrice de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'cord',...
                 [" A matrix associated to always active blocks."
                  " The first column contains the block number, "
                  " and the second, the event code by which the "
                  " block should be called.";
                  " ";
                  " Size : ncord.";
                  " Type : matrix of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'cord',...
//                  [" Une matrice associé aux blocs activés en";
//                   " permanence. La première colonne contient";
//                   " les numéros des blocs et la deuxième le"
//                   " code de l''événement par lequel le bloc"
//                   " sera appelé pendant la simulation"
//                   " ";
//                   " Taille : ncord.";
//                   " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'oord',...
                 [" Subset of {cord}. Blocks of that matrix "
                  " have outputs which affect computation of"
                  " continuous state derivatives"
                  " ";
                  " Size : noord.";
                  " Type : matrix of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'oord',...
//                  [" Sous-ensemble de {cord}. Les blocs de cette";
//                   " matrice ont des sorties qui affectent le "
//                   " calcul des états continus.";
//                   "  ";
//                   " Taille : noord.";
//                   " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zord',...
                 [" Subset of {zord}. Blocks of that matrix "
                  " have outputs which affect computation of"
                  " zero-crossing surfaces."
                  " ";
                  " Size : nzord.";
                  " Type : matrix of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'zord',...
//                  [" Sous-ensemble de {cord}. Les blocs de cette";
//                   " matrice ont des sorties qui affectent le "
//                   " calcul des détections de surface.";
//                   "  ";
//                   " Taille : nzord.";
//                   " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'critev',...
                 [" A vector of size equal to the number of "
                  " activations and containing zeros and "
                  " ones. The value one indicates that the "
                  " activation is critical in the sense that"
                  " the continuous-time solver must be cold "
                  " restarted.";
                  " ";
                  " Size : number of source of activation.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'critev',...
//                  [" Un vecteur de taille égale au nombre";
//                   " d''activation. Ce vecteur est composé de";
//                   " 0 et de 1 qui indiquent si des événements";
//                   " sont critiques dans le sens où le solveur";
//                   " doit redémarrer à froid.";
//                   " ";
//                   " Taille : nombre de sources d''activation.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'nb',...
                 [" Number of blocks. Note that the number of"
                  " blocks may differ from the original number"
                  " of blocks in the diagram because {c_pass2}"
                  " may duplicate some conditional blocks."
                  " ";
                  " Size : 1";
                  " Type : integer."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'nb',...
//                  [" Nombre de blocs. Notez que le nombre de";
//                   " blocs peut être différent du nombre";
//                   " original de blocs dans le diagramme";
//                   " car la fonction {c_pass2} du compilateur";
//                   " peut dupliquer des blocs synchrones.";
//                   " "
//                   " Taille : 1";
//                   " Type : entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ztyp',...
                 [" A vector of size equal to the number of blocks."
                  " A 1 entry indicates that the block may have "
                  " zero-crossings, even if it doesn''t in the "
                  " context of the diagram.  Usually not used by "
                  " the simulator. ";
                  " ";
                  " Size : number of blocks.";
                  " Type : column vector of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'ztyp',...
//                  [" Un vecteur de taille égale au nombre de blocs.";
//                   " Une entrée égale à 1 indique que le bloc possède";
//                   " une détection de surface."
//                   " ";
//                   " Taille : nombre de source d''activations + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'nblk',...
                 ["Not used. Set to {nb}.";
                  " ";
                  " Size : 1";
                  " Type : integer."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'nblk',...
//                  ["Pas utilisé. Mis à {nb}.";
//                   " ";
//                   " Taille : 1";
//                   " Type : entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ndcblk',...
                 ["Not used.";
                  " ";
                  " Size : -";
                  " Type : integer."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'ndcblk',...
//                  ["Pas utilisé.";
//                   " ";
//                   " Taille : -";
//                   " Type : entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'subscr',...
                 [" Not used.";
                  " ";
                  " Size : 0";
                  " Type : empty real."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'subscr',...
//                  ["Pas utilisé.";
//                   " ";
//                   " Taille : 0";
//                   " Type : vide."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'funtyp',...
                 [" A vector of size equal to the number of blocks"
                  " indicating the type of the computational function"
                  " of the block. Block type can be 0 through 5."
                  " Currently only type 4 (C language) and type 5"
                  " (Scilab language) computational functions should"
                  " be used. But older blocks can also be used.";
                  " ";
                  " Size : number of blocks.";
                  " Type : column vector of integer."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'funtyp',...
//                  [" Un vecteur de taille égale au nombre de blocs";
//                   " qui indique le type de la fonction de calcul.";
//                   " Le type de bloc peut aller de 0 à 5.";
//                   " Les blocs courants de Scicos sont les blocs";
//                   " de type 4 (bloc C) et de type 5 (bloc scilab).";
//                   " Les anciens blocs restent compatibles.";
//                   " ";
//                   " Taille : nombre de blocs.";
//                   " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'iord',...
                 [" A matrix associated to blocks that"
                  " must be activated at the start of "
                  " the simulation. This includes blocks "
                  " inheriting from constant blocks and "
                  " always active blocks.";
                  " ";
                  " Size : niord.";
                  " Type : matrix of integers."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'iord',...
//                  [" Une matrice qui associe les blocs à activer ";
//                   " au début de la simulation. Celà inclue les";
//                   " blocs {Constante} et les blocs activés en";
//                   " permanence.";
//                   " ";
//                   " Taille : niord.";
//                   " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'labels',...
                 [" A string vector of size equal"
                  " to the number of blocks "
                  " containing block labels.";
                  " ";
                  " Size : numbers of blocks.";
                  " Type : column vector of strings."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'labels',...
//                  [" Un vecteur de chaîne de caractères de";
//                   " taille égale au nombre de blocs "
//                   " contenant les labels des blocs.";
//                   " ";
//                   " Taille : nombre de blocs.";
//                   " Type : vecteur colonne de chaînes de caractères."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'modptr',...
                 [" A vector pointer to the block modes.";
                  " ";
                  " Size : number of blocks + 1.";
                  " Type : column vector of integer."])

//   %scs_help_sim=scicos_help(%scs_help_sim,...
//                  'modptr',...
//                  [" Un vecteur pointant sur les modes des blocs";
//                   " ";
//                   " Taille : nombre de blocs + 1.";
//                   " Type : vecteur colonne de nombre entiers."])

  //==========================================================

  //Enter here traduction for initial scicos tables ==========
  if ~exists('LANGUAGE') then global LANGUAGE, end

  if LANGUAGE=='fr' then
  //****** scicos_state ******/
  //**************************/
  %scs_help_state=scicos_help(%scs_help_state,...
                 'state',...
                  [" Une liste typée scilab de type {xcs}. Cette";
                   " liste contient tous les états du diagramme qui";
                   " vont évoluer pendant la simulation.";
                   " ";
                   " state contient {x}, {z}, {oz}, {iz}, "
                   " {tevts}, {evtspt}, {pointi} et {outtb}."
                   " ";
                   " Taille : 9.";
                   " Type : tlist scilab."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'x',...
                 [" Le registre des états continus qui est obtenu";
                  " en concatenant tous les états continus de tous";
                  " les blocs."
                  " ";
                  " Taille : total des tailles de tous les registres des états continus.";
                  " Type : vecteur colonne de nombres réels."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'z',...
                 [" Le registre des états discrets qui est obtenu";
                  " en concatenant tous les états discrets de tous";
                  " les blocs."
                  " ";
                  " Taille : total des tailles de tous les registres des états discrets.";
                  " Type : vecteur colonne de nombres réels."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'oz',...
                 [" La liste des états objets discrets obtenue";
                  " par la concaténation de tous les états objets";
                  " de tous les blocs.";
                  " ";
                  " Taille : total des tailles de toutes les listes des états objet.";
                  " Type : liste scilab."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'iz',...
                 [" Vecteur de taille égale au nombre de blocs.";
                  " Ce vecteur est utilisé pour enregistrer les"
                  " pointeurs des registres de travail (work)"
                  " Si un bloc fait une allocation du registre work"
                  " à l''initialisation (flag=4) alors le pointeur"
                  " associé est enregistré ici."
                  " ";
                  " Taille : nombre de blocs.";
                  " Type : vecteur colonne de nombres réels."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'tevts',...
                 [" Un vecteur de taille égale au nombre";
                  " de sources d''activation. C''est une table"
                  " contenant les dates des événements "
                  " programmés dans {evtspt}."
                  " ";
                  " Taille : nombre de sources d''activation.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'evtspt',...
                 [" Un vecteur de taille égale au nombre";
                  " de sources d''activation. C''est une table"
                  " d''événements."
                  " ";
                  " Taille : nombre de sources d''activation.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'pointi',...
                 [" Le numéro du prochain événement";
                  " ";
                  " Taille : 1";
                  " Type : entier."])

  %scs_help_state=scicos_help(%scs_help_state,...
                 'outtb',...
                 [" Liste scilab contenant tous les registres";
                  " de sortie des blocs. Chaque éléments de cette"
                  " liste contient des données typées matricielles."
                  " ";
                  " Taille : nombre de ports réguliers de sortie.";
                  " Type : liste scilab contenant des matrices."])

  //****** scicos_sim ******/
  //************************/

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'sim',...
                 [" Une liste typée scilab de type {scs}. Cette";
                  " liste contient des tableaux statiques issus"
                  " du résultat de la compilation. Ces tableaux"
                  " n''évoluent pas pendant la simulation."
                  " ";
                  " Taille : 33";
                  " Type : tlist scilab."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'funs',...
                 [" Une liste qui contient les noms des";
                  " fonctions de calculs ou des fonctions";
                  " scilab";
                  " ";
                  " Taille : nombre de blocs.";
                  " Type : liste scilab."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'xptr',...
                 [" Un vecteur pointant vers le registre des états";
                  " continus {x}. Le registre des états continus";
                  " du bloc {i} peut-être retrouvé par :";
                  " {state.x(sim.xptr(i):sim.xptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zptr',...
                 [" Un vecteur pointant vers le registre des états";
                  " discret {z}. Le registre des états discrets";
                  " du bloc {i} peut-être retrouvé par :";
                  " {state.z(sim.zptr(i):sim.zptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ozptr',...
                 [" Un vecteur pointant vers le registre des états";
                  " objets discret {oz}. Le registre des états objets";
                  " du bloc {i} peut-être retrouvé par :";
                  " {state.oz(sim.ozptr(i):sim.ozptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zcptr',...
                 [" Un vecteur pointant vers le registre des";
                  " détections de surfaces. Ce vecteur indique";
                  " par bloc le nombre de passages à zéro utilisé.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'inpptr',...
                 [" (sim.inpptr(i+1)-sim.inpptr(i)) donne le nombre";
                  " de ports réguliers d''entrée du bloc i.";
                  " inpptr(i) pointe vers le début des ports d''entrée"
                  " du bloc i et les connections à ces ports peuvent"
                  " retrouvées par la table d''indirection {inplnk}."
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'outptr',...
                 [" (sim.outptr(i+1)-sim.outptr(i)) donne le nombre";
                  " de ports réguliers de sortie du bloc i.";
                  " outptr(i) pointe vers le début des ports de sortie"
                  " du bloc i et les connections à ces ports peuvent"
                  " retrouvées par la table d''indirection {outlnk}."
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'inplnk',...
                 [" (cpr.sim.inplnk(cpr.sim.inpptr(i)-1+j))";
                  " est l''index du lien connecté à l''entrée j";
                  " du bloc i, où j va de 1 jusqu''à";
                  " (cpr.sim.inpptr(i+1)-cpr.sim.inpptr(i))";
                  " ";
                  " Taille : nombre total de port réguliers d''entrée.";
                  " Type : vecteur colonne de nombre entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'outlnk',...
                 [" (cpr.sim.outlnk(cpr.sim.outptr(i)-1+j))";
                  " est l''index du lien connecté à la sortie j";
                  " du bloc i, où j va de 1 jusqu''à";
                  " (cpr.sim.outptr(i+1)-cpr.sim.outptr(i))";
                  " ";
                  " Taille : nombre total de port réguliers de sortie.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'rpar',...
                 [" Vecteur des paramètres réels qui est";
                  " obtenu en concatenant les registres"
                  " des paramètres réels de tous les blocs."
                  " ";
                  " Taille : nombre total de paramètres réels.";
                  " Type : vecteur colonne de nombre réels."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'rpptr',...
                 [" Un vecteur pointant vers le registre des";
                  " paramètres réels {rpar}. Les paramètres "
                  " réels du bloc i peuvent être retrouvés par :";
                  " {sim.rpar(sim.rpptr(i):sim.rpptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ipar',...
                 [" Vecteur des paramètres entiers qui est";
                  " obtenu en concatenant les registres"
                  " des paramètres entiers de tous les blocs."
                  " ";
                  " Taille : nombre total de paramètres entiers.";
                  " Type : vecteur colonne de nombre réels."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ipptr',...
                 [" Un vecteur pointant vers le registre des";
                  " paramètres entier {ipar}. Les paramètres "
                  " entiers du bloc i peuvent être retrouvés par :";
                  " {sim.ipar(sim.ipptr(i):sim.ipptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'opar',...
                 [" Liste des paramètres objets qui est";
                  " obtenue en concatenant les listes"
                  " des paramètres objets de tous les blocs."
                  " ";
                  " Taille : nombre total de paramètres objets.";
                  " Type : liste scilab."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'opptr',...
                 [" Un vecteur pointant vers la liste des";
                  " paramètres objets {opar}. Les paramètres "
                  " entiers du bloc i peuvent être retrouvés par :";
                  " {sim.opar(sim.opptr(i):sim.opptr(i+1)-1)}.";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'clkptr',...
                 [" Un vecteur pointant vers les ports ";
                  " d''activation de sortie."
                  " (cpr.sim.clkptr(i):cpr.sim.clkptr(i+1)-1)"
                  " donne les numéros des ports de sortie"
                  " événementiel du bloc i."
                  " "
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ordptr',...
                 [" Un vecteur pointant vers {ordclk}";
                  " (cpr.sim.ordptr(i):cpr.sim.ordptr(i+1)-1)"
                  " pointe vers la zone de ordclk qui"
                  " indique les numéros des blocs activés par"
                  " le port événementiel numérotés i."
                  " ";
                  " Taille : nombre de source d''activations + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'execlk',...
                 ["Non utilisé.";
                  " ";
                  " Taille : ";
                  " Type : "])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ordclk',...
                 [" Une matrice associé aux blocs activés par";
                  " événements. La première colonne contient";
                  " les numéros des blocs et la deuxième le"
                  " code de l''événement par lequel le bloc"
                  " sera appelé pendant la simulation"
                  " ";
                  " Taille : nombre total de bloc sommé par source d''activations.";
                  " Type : matrice de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'cord',...
                 [" Une matrice associé aux blocs activés en";
                  " permanence. La première colonne contient";
                  " les numéros des blocs et la deuxième le"
                  " code de l''événement par lequel le bloc"
                  " sera appelé pendant la simulation"
                  " ";
                  " Taille : ncord.";
                  " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'oord',...
                 [" Sous-ensemble de {cord}. Les blocs de cette";
                  " matrice ont des sorties qui affectent le "
                  " calcul des états continus.";
                  "  ";
                  " Taille : noord.";
                  " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'zord',...
                 [" Sous-ensemble de {cord}. Les blocs de cette";
                  " matrice ont des sorties qui affectent le "
                  " calcul des détections de surface.";
                  "  ";
                  " Taille : nzord.";
                  " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'critev',...
                 [" Un vecteur de taille égale au nombre";
                  " d''activation. Ce vecteur est composé de";
                  " 0 et de 1 qui indiquent si des événements";
                  " sont critiques dans le sens où le solveur";
                  " doit redémarrer à froid.";
                  " ";
                  " Taille : nombre de sources d''activation.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'nb',...
                 [" Nombre de blocs. Notez que le nombre de";
                  " blocs peut être différent du nombre";
                  " original de blocs dans le diagramme";
                  " car la fonction {c_pass2} du compilateur";
                  " peut dupliquer des blocs synchrones.";
                  " "
                  " Taille : 1";
                  " Type : entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ztyp',...
                 [" Un vecteur de taille égale au nombre de blocs.";
                  " Une entrée égale à 1 indique que le bloc possède";
                  " une détection de surface."
                  " ";
                  " Taille : nombre de source d''activations + 1.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'nblk',...
                 ["Pas utilisé. Mis à {nb}.";
                  " ";
                  " Taille : 1";
                  " Type : entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'ndcblk',...
                 ["Pas utilisé.";
                  " ";
                  " Taille : -";
                  " Type : entier."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'subscr',...
                 ["Pas utilisé.";
                  " ";
                  " Taille : 0";
                  " Type : vide."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'funtyp',...
                 [" Un vecteur de taille égale au nombre de blocs";
                  " qui indique le type de la fonction de calcul.";
                  " Le type de bloc peut aller de 0 à 5.";
                  " Les blocs courants de Scicos sont les blocs";
                  " de type 4 (bloc C) et de type 5 (bloc scilab).";
                  " Les anciens blocs restent compatibles.";
                  " ";
                  " Taille : nombre de blocs.";
                  " Type : vecteur colonne de nombre entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'iord',...
                 [" Une matrice qui associe les blocs à activer ";
                  " au début de la simulation. Celà inclue les";
                  " blocs {Constante} et les blocs activés en";
                  " permanence.";
                  " ";
                  " Taille : niord.";
                  " Type : matrice de nombres entiers."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'labels',...
                 [" Un vecteur de chaîne de caractères de";
                  " taille égale au nombre de blocs "
                  " contenant les labels des blocs.";
                  " ";
                  " Taille : nombre de blocs.";
                  " Type : vecteur colonne de chaînes de caractères."])

  %scs_help_sim=scicos_help(%scs_help_sim,...
                 'modptr',...
                 [" Un vecteur pointant sur les modes des blocs";
                  " ";
                  " Taille : nombre de blocs + 1.";
                  " Type : vecteur colonne de nombre entiers."])
  end
  //==========================================================

  //Build final Scicos Help list =============================

  //** update scs_help_scilst lists
  %scs_help_scilst.diagr=%scs_help_diagr;       //** diagr
  %scs_help_scilst.params=%scs_help_params;     //** params
  %scs_help_scilst.model=%scs_help_model;       //** model
  %scs_help_scilst.graphics=%scs_help_graphics; //** graphics
  %scs_help_scilst.link=%scs_help_link;         //** link
  %scs_help_scilst.state=%scs_help_state;       //** state
  %scs_help_scilst.sim=%scs_help_sim;           //** sim

  %scicos_help=tlist(['sch','menu','scilst'],..
                      %scs_help_menu,%scs_help_scilst);
  //==========================================================

  //** This is the END, the END my friend". By The Doors, Apocalypse Now.
endfunction
