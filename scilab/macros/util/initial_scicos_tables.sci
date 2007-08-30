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

  //Scicos palettes =================================================================
  //**
  pal_names = ['Sources'    ;'Sinks'           ;'Linear'          ;'Non_linear';
               'Matrix'     ;'Integer'         ;'Events'          ;'Threshold' ;
               'Others'     ;'Branching'       ;'Electrical'      ;'ThermoHydraulics';
               'OldBlocks'  ;'DemoBlocks'];
  scicos_pal= [pal_names  'SCI/macros/scicos/'+pal_names+'.cosf']
  //Scicos palettes loading ===========================================
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
	      'Quit' ];
  
  Diagram  = ['Diagram',..
              'Replot',..
	      'Purge',..
	      'Set Diagram Info',..
	      'Region to Super Block',..
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
              'Browser',..
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
	      'Navigator'..
	      'Code Generation',..
	      'Shortcuts',..
	      'Calc'];
  
  Help     = ['Help',..
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
			     'Replace',..
			     'Flip',..
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
		 'g','Grid']
  
  //Scicos Modelica librabry path definitions==============================
  modelica_libs='SCI/macros/scicos_blocks/'+['Electrical','Hydraulics'];

  //add TMPDIR/Modelica for generic modelica blocks
  status=mkdir(TMPDIR,'Modelica');
  if isdir(TMPDIR+'/Modelica') then 
    modelica_libs=[modelica_libs,TMPDIR+'/Modelica'];
  end

  //Scicos Menus Help definitions===========================================
  
  %scicos_help = tlist(..
		     ['sch','Display mode','Window','Background color','Default link colors',..
		      'ID fonts','Aspect','Add color','Focus','Shift','Zoom in','Zoom out',..
		      'Fit diagram to figure','View','Help','Calc','Palettes','Context',...
                      'Cut','Copy','Paste','Set grid',..
		      'Smart Move','Move','Duplicate',..
		      'Duplicate Region','Replace','Align','Link','Delete','Delete Region',..
		      'Add new block','Flip','Rotate Left','Rotate Right',..
                      'Undo','Setup','Compile','Eval','Debug Level','Run',..
		      'Replot','New','Region to Super Block','Purge','Rename','Save',..
		      'Save As','Load','Load as Palette','Save as Palette','Save as Interf Func',..
		      'Set Diagram Info','Navigator','Quit','Open/Set','Resize','Icon',..
		      'Color','Label','Get Info','Identification','Documentation',..
		      'Code Generation','Export','Export All','Pal editor','Icon Editor','Shortcuts'],..
		     ["Display mode can be 0 or 1. In mode 1, scicos editor uses the";
		      "pixmap option of Scilab graphics to create   a backing";
		      "store for the graphics used for displaying scicos diagrams. ";
		      "By default, Display mode is 0. Under Windows operating system";
		      "the backing store is handled in a different way. "],..
		     [' In the active editor Scicos window, clicking on the ';
		      ' Window menu item invokes a dialog box that allows you to change ';
		      ' window dimensions.'],..
		     [' This menu allows to change the background and default foreground';' colors.'],..
		     [' This menu allows to change the default color for regular ';' and activation links.'],..
		     [' This menu allows to change the font used to write in the block';
		      ' identifications (see ""Set block ID"" menu).'],..
		     [' This menu allows to select 3D shape for blocks and ';' associated parameters.'],..
		     [' This menu allows to add new color to the diagram private';
		      ' color map. Added colors are stored in the diagram data structure.'],..
		     [' Focus menu allows to select a zone (click left, drag zone, click';
		      ' to select) which is focused on.';
		      ' To change focus back use ""Zoom in"" menu.'],..
		     [' To shift the diagram to left, right, up or down,';
		      ' select this menu item, then click on the point you want ';
		      ' to appear in the middle of the graphics window. '],..
		     [' When you select this menu item the diagram is zoomed in ';'by a factor of 10%.'],..
		     [' When you select this menu item the diagram is zoomed out ';'by a factor of 10%.'],..
		     [' When you select this menu item the diagram is fit in the figure'],..
		     ['View/unview grid'],..
		     [' To get help on an object or menu buttons,';
		      ' select first Help menu item and then on ';
		      ' the selected object or menu item.'],..
		     [' When you select this menu item you switch Scilab to ';
		      ' the pause mode (see the help on pause).';
		      ' In the Scilab main window and you may enter Scilab instructions';
		      ' to compute whatever you want.';
		      ' to go back to Scicos you need enter the ""return"" or';
		      ' ""[...]=return(...)"" Scilab instruction.';
		      ' ';
		      ' If you use ""[...]=return(...)"" Scilab instruction take care';
		      ' not to modify Scicos variables such as ""scs_m"",""scs_gc"",';
		      ' ""menus"",""datam"",...';
		      ' ';
		      ' If you have modified Scicos graphic window you may restore it ';
		      ' using the Scicos ""Replot"" menu.'],..
		     ' Select the Palettes menu item to open a predefined palette.',..
		     [' When you select this menu item you obtain a dialog to';
		      ' enter Scilab instructions for defining symbolic Scicos parameters';
		      '  used in block definitions or to do whatever you want';
		      ' ';
		      ' These instructions will be evaluated each time the diagram ';
		      ' is loaded.';
		      ' ';
		      ' If you  change the value of a symbolic Scicos parameters in ';
		      ' the context, all the blocks are updated (Eval is performed).'],..
		     [' Cut is used to remove the selected object from the diagram";
		      ' and keep a copy in the clipboard if the object is a block.'],..
		     [' Copy is used to place a copy of the selected object';
		      ' in the clipboard if the object is a block.'],..
		     [' Paste places the object in the Clipboard in the diagram.'],..
		     [' Set the grid'],..
		     [' To move a block in  the active editor Scicos window';
		      ' or in edited palette keeping horizontal and vertical';
		      ' links, select first the ""Smart Move"" menu item, ';
		      ' then click on the selected block, link segment or link';
		      ' corner, drag the mouse to the desired new  position ';
		      ' and click left again to fix the position.';
		      ' ';
		      ' Right click cancel the move action.'],..
		     [' To move a block in  the active editor Scicos window';
		      ' or in edited palette,';
		      ' select first the Move menu item, ';
		      ' then click on the selected block, link segment or link';
		      ' corner, drag the mouse to the desired new block position ';
		      ' and click left again to fix the position.';
		      ' ';
		      ' Right click cancel the move action.'],..
		     ['To duplicate a block in the active editor Scicos window';
		      ' select first the Duplicate menu item, then';
		      ' click (with left button) on the to-be-copied block';
		      ' in Scicos windows or in a palette) ,  and';
		      ' finally click left where you want the copy';
		      ' be placed in the active editor Scicos window.';
		      ' ';
		      ' The lower left corner of the block is placed';
		      ' at the selected point.';
		      ' ';
		      ' Right click cancel the copy action.'],..
		     ['To copy a region in the active editor Scicos window';
		      ' select first the Copy menu item, then';
		      ' click (with right button) on a corner of the desired';
		      ' region (in Scicos windows or in a palette), drag to ';
		      ' define the region, click to fix the region  and';
		      ' finally click left where you want the copy.';
		      ' to be placed in the active editor Scicos window.';
		      ' NOTE: If source diagram is big be patient, region selection ';
		      ' may take a while.';
		      ' ';
		      ' The lower left corner of the block is placed';
		      ' at the selected point.';
		      ' ';
		      ' Right click cancel the copy action.'],..
		     [' To replace a block in the active editor Scicos window';
		      ' select first the Replace menu item, then';
		      ' select the replacement block (in Scicos window or in a';
		      ' palette), and  finally click on the to-be-replaced block.'],..
		     [' To obtain nice diagrams, you can align ports of';
		      ' different blocks, vertically and horizontally.';
		      ' Select first  the Align menu item, then click on the first';
		      ' port and finally on the second port.';
		      ' The block corresponding to the second port is moved.';
		      ' ';
		      ' A connected block cannot be aligned.'],..
		     [' To connect an output port to an input port,';
		      ' select first the Link menu item, then click on the output';
		      ' port, drag, click left on each intermediate points';
		      ' and finally click left on the input port.';
		      ' ';
		      ' To split a link, select first  the Link menu item,';
		      ' then click left on the link where the split should be placed,';
		      ' drag, click left on each intermediate points';
		      ' and finally click left on the input port.';
		      ' ';
		      ' Right click cancel the link action';
		      ' ';
		      ' Only one link can go from and to a port.';
		      ' Link color can be changed directly by clicking';
		      ' on the link.';
		      ' ';
		      ' This menu remains active until user choose another one.'],..
		     ['To delete  blocks or a links, select first the Delete';
		      ' menu item, then click successively on the selected objects';
		      '(with left button).';
		      ' ';
		      ' When you delete a block all links connected to it';
		      ' are deleted as well.';],..
		     ['To delete a blocks in  a region, select first  the Delete Region';
		      ' menu item, then click  on a corner of the ';
		      ' desired region, drag to define the region, and click left to ';
		      ' fix the region. All connected links will be destroyed as';
		      ' well';
		      ' ';
		      ' Right click instead of left cancels the delete action.'],..
		     [' To add a newly defined block to the current palette or diagram';
		      ' select first this menu item, A dialog box will pop up ';
		      ' asking for the name of the GUI function associated ';
		      ' with the block. If this function is not already loaded';
		      ' it was search in the current directory. The user may then';
		      ' click at the desired position of the block icon .'],..
		     [' To reverse the positions of the (regular) inputs';
		      ' and outputs of a block placed on its sides,';
		      ' select the Flip menu item first and then click on the';
		      ' selected block. This does not affect the order,';
		      ' nor the position of the input and output event';
		      ' ports which are numbered from left to right.';
		      ' ';
		      ' A connected block cannot be flipped.'],..
		     [''],.. //rotate left
		     [''],.. //rotate roght
		     [' Select the Undo menu item to undo the last edit operation.';
		      ' It is not possible to undo more!.'],..
		     [' In the main Scicos window, clicking on the Setup menu item';
		      ' invokes a dialog box that allows you to change ';
		      ' integration parameters: ';
		      ' '
		      '   - Final integration time (integration ends at'
		      '           this time, it always starts from 0), ';
		      ' '
		      '   - Real time scaling (forces real time simulation'
		      '           by setting Scicos unit of time to 1 second), '
	              ' '
		      '   - Absolute and relative error tolerances (solver properties), '
		      ' '
		      '   - Time tolerance (the smallest time interval for which ';
		      '         the ode solver is used to update continuous states), ';
		      ' '
		      '   - Max integration time interval (the maximum time interval' 
		      '     for each call to solver, it must be reduced if the error';
		      '     message '"too many calls'" is encountered), '
		      ' '
		      '   - Solver (choose the numerical solver to be used), '
		      ' '
		      '   Max step size (max time step taken by solver).'],..
		     [' select the Compile menu item to compile the block diagram.';
		      ' This menu item need never be used since compilation is';
		      ' performed automatically, if necessary, before';
		      ' the beginning of every simulation (Run menu item).';
		      ' ';
		      ' Normally, a new compilation is not needed if only';
		      ' system parameters and internal states are modified.';
		      ' In some cases however these modifications are not';
		      ' correctly updated and a manual compilation may be';
		      ' needed before a Restart or a Continue.';
		      ' Please report if you encounter such a case.'],..
		     [' All dialogs user answers may be Scilab instructions';
		      ' they are evaluated immediately and stored as character strings.';
		      ' select this menu item to have them re-evaluated according to';
		      ' new values of underlying Scilab variables. ';
		      ' ';
		      ' These underlying Scilab variables may be user global variables';
		      ' defined before Scicos was launch, They may also be defined in';
		      ' by the Scicos context (see Context menu item).'],..
		     [' Set the debug level: 0 (no debugging), 1 (light';
		      ' debugging information printout), 2 (more information';
		      ' printout and execution of Debug Block if any in diagram)'],..
		     [' Select the Run menu item to start the simulation.';
		      ' If the system has already been simulated, a';
		      ' dialog box appears where you can choose to Continue,';
		      ' Restart or End the simulation.';
		      ' ';
		      ' You may interrupt the simulation by clicking on the ';
		      ' ""stop"" button, change any of the block parameters';
		      ' and continue the simulation with the new values.'],..
		     [' Select the Replot menu item to replot the content of';
		      ' the graphics window. Graphics window stores complete';
		      ' history of the editing session in memory.';
		      ' ';
		      ' Replot is useful for ''cleaning'' this memory.'],..
		     [' Clicking on the New menu item loads an empty diagram in the';
		      ' active editor Scicos window. If the previous content of the';
		      ' window is not saved, it will be lost.'],..
		     [' This menu allows to transform a rectangular region of the';
		      ' current diagram by a super block.';
		      ' Click  on a corner of the region , drag an click left to';
		      ' fix the region (left click cancels selection)';
		      ' ';
		      ' Region is replaced by a super block and links are redrawn.'],..
		     [' select the Purge menu item to get a clean data structure:';
		      ' If diagram has been hugely modified many deleted blocks';
		      ' may remain in the data structure. It may be  useful to';
		      ' suppress then before saving.'],..
		     [' This menu allows to change the diagram name. An editable';'  dialog box opens.'],..
		     [' Select the save menu item to save the block diagram';
		      ' in a binary file already selected by a previous';
		      ' select the Save As menu item. If you select this';
		      ' menu item and you have never clicked on the Save As';
		      ' menu item, the diagram is saved in the current directory';
		      ' as <window_name>.cos where <window_name> is the name';
		      ' of the window appearing on top of the window (usually';
		      ' Untitled or Super Block).'
		      ' '
		      ' The .cos binary files are machine independent.'],..
		     [' Select the Save As menu item to save the block diagram';
		      ' or palette in a  file. A dialog box allows choosing ';
		      ' the file which must have a .cos or .cosf extension. The diagram';
		      ' takes the name of the file (without the extension).';
		      ' ';
		      ' If extension is "".cosf"" an ASCII  formatted save is performed';
		      ' instead of binary save. Formatted save is slower than regular ';
		      ' save.'],..
		     [' Select the Load menu item to load an ASCII or binary file';
		      ' containing a saved block diagram or palette.';
		      ' A dialog box allows user choosing the file.'],..
		     [' select the Load menu item to load an ASCII or binary file';
		      ' containing a saved block diagram as a palette.';
		      ' A dialog box allows user choosing the file.'],..
		     [' Select the Save as Palette menu item to save the block diagram';
		      ' as a palette in a file. A dialog box allows choosing ';
		      ' the file which must have a .cos or .cosf extension. The palette';
		      ' takes the name of the file (without the extension).';
		      ' ';
		      ' If extension is "".cosf"" an ASCII  formatted save is performed';
		      ' instead of binary save. It may take a while';
		      ' ';
		      ' .scilab user file is updated if necessary.'],..
		     [' Select ""the Save as Interf. Func."" menu item to save the ';
		      ' diagram as a new Scicos block. A Scilab function is generated';
		      ' and saved in a  file with "".sci"" extension. File name and path';
		      ' are to be set in a ""File menu"" dialog.'],..
		     [' This menu allows to set user''s diagram information;';
		      ' this info is stored in the diagram data structure';
		      ' and may be used for diagram documentation.';
		      ' ';
		      ' Information format may be redefined by user.'],..
		     [' This experimental menu opens a graphic window with a tree ';
		      ' representation of the super blocks hierarchy. Each node ';
		      ' represents a superblock.';
		      ' ';
		      ' Navigator window is useful to open directly a super-block';
		      ' everywhere in the hierarchy.'],..
		     [' Click on the Exit menu item to close current diagram. ';
		      ' If current diagram is not a Super block, Exit menu closes ';
		      ' Scicos and returns to Scilab session. Save your diagram ';
		      ' or/and your palette before leaving.';
		      ' ';
		      ' File/Close menu has the same effect.'],..
		     [' To change the parameters of a regular block or link, or ';
		      ' to open a super block, select first ';
		      ' this menu item, click next on the desired object.';
		      ' A dialog or an editing window appears';
		      ' which allows you to modify object properties.';
		      ' ';
		      ' It is also possible to select a super block to open clicking';
		      ' on a node of the ""Navigator"" window.'],..
		     [' To change the size of a block , select first this menu item,';
		      ' click next on the desired block. A dialog appear that allows ';
		      ' you to change the width and/or height of the block shape.'],..
		     [' To change the icon of a block, select first this menu item,';
		      ' click next on the desired block. A dialog appear that allows ';
		      ' you to enter Scilab instructions used to draw the icon';
		      ' ';
		      ' You may use the icon_editor menu to generate the Scilab';
		      ' instructions.'],..
		     [' To change the background color of an object, select first ';
		      ' this menu item, click next on the desired object. A dialog appear';
		      ' that allows you to choose the desired color.'],..
		     [' To add a label to block, select first this menu item, click next';
		      ' on the desired block. A dialog appear that allows you to enter ';
		      ' the desired label.';
		      ' labels are used to import data from a block in an other one.'],..
		     [' This menu allows to get information on an object and on ';
		      ' its connection with other diagram objects.';
		      ' ';
		      ' Select this menu and click on an object.'],..
		     [' This menu allows to set an identificator to a link or a block ';
		      ' block identificators are drawn under the block icon. Super blocks';
		      ' input/output ports identificators are replicated over the block';
		      ' shape ports. Links identificators are not displayed';
		      ' ';
		      ' Selecting this menu and clicking on a block or links opens an';
		      ' editable dialog box.'],..
		     [' This menu allows to set or get documentation for a block ';
		      ' ';
		      ' Selecting this menu and clicking on a block  opens an';
		      ' editable dialog box.'],..
		     [' This menu allows to generate the simulation code associated with' 
		      ' a discrete time Superblock.'
		      ''
		      ' The code generation is obtained simply by selecting this menu and  '
		      ' then the desired superblock.'
		      ' '
		      ' If the superblock satisfies the required conditions, a dialog box'
		      ' pops up to ask for a block name, a directory where to put the'
		      ' generated files and for optional libraries requested by the linker.'
		      ' '
		      ' Given this information the code is generated, compiled and linked with '
		      ' Scilab. The superblock is automatically replaced by a new block which '
		      ' implements the generated code. It is then possible to run the modified '
		      ' diagram.'],..
		     [' This menu is used to export Scicos diagram in postscript format.'],..
		     [' This menu is used to export Scicos diagrams in postscript format.'],..
		     [' Pal editor allows you to define and reorder the palettes.'],..
		     [' Icon Editor allows you to define graphically the icon of the block.'],..
		     [' Used to define personalized shortcuts.'])

		     
  //** Scicos "xinfo" messages ===========================================
  //** 		     
  //** "%CmenuTypeOneVector" store the list of the commands/function to be called that require both 'Cmenu' AND '%pt'
  //** menus of type 1 (require %pt)
  %CmenuTypeOneVector =..
     ['Region to Super Block', 'Click, drag region and click (left to fix, right to cancel)'        ;
      'Smart Move',            'Click object to move, drag and click (left to fix, right to cancel)';
      'Move',                  'Click object to move, drag and click (left to fix, right to cancel)';
      'Duplicate',             'Click on the object to duplicate, drag, click (left to copy, right to cancel)';
      'Replace',               'Click on new object , click on object to be replaced';
      'Align',                 'Click on an a port , click on a port of object to be moved';
      'Link',                  'Click link origi-n, drag, click left for final or intermediate points or right to cancel';
      'Delete',                'Delete: Click on the object to delete';
      'Flip',                  'Click on block to be flipped'      ;
      'Rotate Left',           'Click on block to be turned left'  ;
      'Rotate Right',          'Click on block to be turned right' ;
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
      'Identification',        'Click on an object to set or get identification';
      'Resize',                'Click block to resize';
      'Block Documentation',   'Click on a block to set or get it''s documentation'
     ]
    
  //**-----------------------------

//** This is the END, the END my friend". By The Doors, Apocalypse Now.    
endfunction
