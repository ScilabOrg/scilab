function [palettes,windows] = do_palettes(palettes,windows)
//** 19 Jan 2006 
//** 26 Jun 2006 
//** Comments and mods by Simone Mannori 
//**
//      Copyright INRIA
//** 

  //** Call a Tk interactive menu in order to choose a palette 
  kpal = choose(scicos_pal(:,1),'Choose a Palette')
 
  //** if no Palette are choosen the exit 
  if kpal==0 then return,end

  lastwin = curwin ; //** save the current graphic window id 
  
  winpal = find(windows(:,1)==-kpal) 
  
  
  //** First level protection 
  if winpal<>[] then
    
    if ~or(windows(winpal,2)==winsid()) then
      windows(winpal,:)=[]
      winpal=[]
    end
  
  end

  //** Second level protection
  if winpal==[] then  //selected palettes isnt loaded yet
    curwin = get_new_window(windows)
    
    if or(curwin==winsid()) then
      xdel(curwin);
      //xset('window',curwin)
    end
    
    windows=[windows;[-kpal curwin]]
    palettes = add_palette(palettes,scicos_pal(kpal,2),kpal)
    
    if palettes(kpal)==list() then return,end
  
  else //selected palettes is already loaded 
  
    curwin=windows(winpal,2)
  
  end
    
  //
  // xset('window',curwin),
  
  gh_current_window = [];
  gh_current_window = scf(curwin) ; //** open a graphic window and get the handler
  gh_curwin = gh_current_window   ;
  gh_palette = gh_curwin          ;
  
  //** gh_palette.pixmap = "on" ; <------- //** eliminato perche' NON funziona ! 

  //** delete the unuseful menu options 

  if ~MSDOS then
    delmenu(curwin,'3D Rot.')
    delmenu(curwin,'UnZoom')
    delmenu(curwin,'Zoom')
    delmenu(curwin,'Edit')
    delmenu(curwin,'Insert')
  else
     hidetoolbar(curwin)
     // French
        // delmenu(curwin,'&Fichier')
    	delmenu(curwin,'&Editer')
  	delmenu(curwin,'&Outils')
  	delmenu(curwin,'&Inserer')
     // English
  	  // delmenu(curwin,'&File')
        delmenu(curwin,'&Edit')
        delmenu(curwin,'&Tools')
        delmenu(curwin,'&Insert')
  end
  
  xselect(); //** rise the current graphics window 
  
  //** obsolete, removed
  //** xset('alufunction',3) //** copy mode 
  //** if pixmap then xset('pixmap',1),end,xbasc();
  
  //**-------------------------------------------------------
  rect = dig_bound(palettes(kpal));
  if rect==[] then rect=[0 0 400,600],end
  %wsiz=[rect(3)-rect(1),rect(4)-rect(2)];
  //window size is limited to 400 x 300 ajust dimensions
  //to remain isometric.
  
  if %wsiz(1)<400 then 
    rect(1)=rect(1)-(400-%wsiz(1))/2
    rect(3)=rect(3)+(400-%wsiz(1))/2
    %wsiz(1)=400 
  end

  if %wsiz(2)<300 then 
    rect(2)=rect(2)-(300-%wsiz(2))/2
    rect(4)=rect(4)+(300-%wsiz(2))/2
    %wsiz(2)=300 
  end

  %zoom = 1.2
  h = %zoom*%wsiz(2)
  w = %zoom*%wsiz(1)

  if ~MSDOS then h1=h+50,else h1=h,end
  
//  xset('wresize',1); //** xset("wresize",flag). If flag=1 then the graphic is automatically resized
//                     //** to fill the graphics window. 
  
  gh_palette.auto_resize = "on" ; //** 
  
  
//  xset('wpdim',w,h1);//** xset("wpdim",width,height): Sets the width and the height of the current
//                     //** physical graphic window (which can be different from the actual size in 
//                     //** mode wresize 1). This option is not used by the postscript driver.  
  
  gh_palette.figure_size = [w h1] ; 
  
  
//  xset('wdim',w,h)   //** xset("wdim", width, height): Set the width and the height of the current
//                     //** graphics window. This option is not used by the postscript driver. 

  gh_palette.axes_size = [w h];


//  xsetech(wrect=[0 0 1 1] ; frect=rect, arect=[1 1 1 1]/32 ) ; 

  wrect=[0 0 1 1] ; //** default value 
  frect = [rect(1) rect(2) ; rect(3) rect(4)] ; //** vector to matrix conversion       ; 
  arect = [1 1 1 1] / 32 ; //** 1/32 of margin  

  gh_pal_axes = gh_palette.children ;
  
  gh_pal_axes.axes_bounds = wrect ; //** default : axes_bounds = [0,0,1,1] = [xmin ymin xmax ymax] 
 
  gh_pal_axes.data_bounds = frect ; //** default : data_bounds = [0,0 ; 1,1] = [xmin ymin ; xmax ymax ]
  
  gh_pal_axes.margins     = arect ; //** arect
 

//** eliminating the load / save 

  // graph=TMPDIR+'/'+scicos_pal(kpal,1)+'.pal'

  //Check if the graph file exists
  // [u,ierr]=file('open',graph,'old')
  /// if u<>[] then file('close',u);end

  // if ierr<>0 then
    
    options = palettes(kpal).props.options
    
    // set_background()
    //** I switch to the direct acces 
    gh_palette.background = options.Background(1) ; //** "options" is sub structure of scs_m  
    
    
    if ~set_cmap(palettes(kpal).props.options('Cmap')) then 
	    palettes(kpal).props.options('3D')(1)=%f //disable 3D block shape 
    end
    
    // drawlater();
    
      drawobjs( palettes(kpal)); //** draw all the object of the palettes 
    
    // drawnow();
    
    // show_pixmap();  
      
      
    //** if pixmap then xset('wshow'),end


//** eliminating the load / save 
  // xsave(graph)
  // else
  //  xload(graph)
  // xname(palettes(kpal).props.title(1))
  //
  // end
  
  
  xinfo('Palette: may be used to copy  blocks or regions')  
  
  //** if pixmap then xset('wshow'),end

  //** xset('window',lastwin)
  
  set ("current_figure" , lastwin ); //** new graphic 

endfunction
