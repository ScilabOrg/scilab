function [scs_m] = do_stupidmove(%pt, scs_m)
// Copyright INRIA
// get a scicos object to move, and move it with connected objects
//!
//  This file contains all the functions relatives to the object (Block Link and Text)
//
//**
//** 28 Jun 2006 : restart :(
//** 21 Aou 2006 : move W , W/O link equalization  
//** 23 Nov 2006 : introduce some mechanism to limit the "off window" object move

// Acquire the current window
  gh_curwin = gh_current_window ;
// get block to move
  rela = 0.1   ; //** "relative object position tollerance"
  win  = %win ;  //** window id

  xc = %pt(1) ; //** recover mouse position at the last event
  yc = %pt(2) ;

  //** look for a valid object 
  [k, wh, scs_m] = stupid_getobj(scs_m,[xc;yc]) ; 
  
  //** "k" is the object index in the data structure "scs_m"
  if k==[] then
    return
  end ;//** if NO object found -> exit

  scs_m_save = scs_m ; //** make a backup of the data structure

  //**-------------------------------------------------------------------
  if typeof(scs_m.objs(k))=='Block' | typeof(scs_m.objs(k))=='Text' then
    //**-------------------- Block --------------------------------------
      needreplay = replayifnecessary() ; //** to be removed later (obsolete)
      //** scs_m , k (scs_m object index), xc yc (mouse coodinate of the last valid event)
      scs_m = stupid_moveblock(scs_m, k, xc, yc)  ;

  elseif typeof(scs_m.objs(k))=='Link' then
    //**------------------- Link ------------------------------
    //** scs_m , k (scs_m object index), xc yc (mouse coordinate of the last valid event)
    scs_m = stupid_movecorner(scs_m, k, xc, yc, wh) ; //** see below in this file

  end
  //**------------------------------------------------------------------

  //** check if the windows was find closed by the previous function calls 
  if Cmenu=='Quit' then
    //active window has been closed
    [%win,Cmenu] = resume(%win,Cmenu)
  end

  [scs_m_save,enable_undo,edited,nc_save,needreplay] = resume(scs_m_save,%t,%t,needcompile,needreplay)
endfunction
//**------------------------------------------------------------------------------------------------------
//**
//********************************************************************************************************
//
//  ---------------------------- Move Blocks and connected Link(s) --------------------------------------- 
//
// Move  block k and modify connected links if any
// look at connected links
//

function scs_m = stupid_moveblock(scs_m, k, xc, yc)
  //**                            scs_m ,k (scs_m object index), xc yc (mouse coodinate of the last valid event) 
  //**
  //**----------------------------------------------------------------------------------
  //** the code below is modified according the new graphics API
  gh_curwin = gh_current_window ;

  //** at this point I need to build the [scs_m] <-> [gh_window] datastructure
  //** I need an equivalent index for the graphics

  o_size = size (gh_curwin.children.children ) ; //** the size:number of all the object

  //** "k" is the object index in the data structure "scs_m"
  //** compute the equivalent "gh_k" for the graphics datastructure
  //gh_k = o_size(1) - k + 1 ; //** semi empirical equation :)
  gh_k=get_gri(k,o_size(1))
  gh_blk = gh_curwin.children.children(gh_k);
  gh_blk_save = gh_blk; //to store handle of graphic obj

  //**-----------------------------------------------------------------------------------
  //** Acquire axes phisical limits (visible limits are smaller)

  figure_axes_size = gh_curwin.axes_size ; //** size in pixel
  x_f = figure_axes_size(1) ;
  y_f = figure_axes_size(2) ;

  [x1_f, y1_f, rect_f] = xchange([0,x_f],[0,y_f],"i2f"); //** convert to local coordinate

  x_min = x1_f(1) ; x_max = x1_f(2) ; //** hor. limits
  y_min = y1_f(2) ; y_max = y1_f(1) ; //** ver. limits (inverted because the upper left corner effect)

  //**-----------------------------------------------------------------------------------

  o = scs_m.objs(k) ;

  xx=[];yy=[];ii=[];clr=[];mx=[];my=[]

  // build movable segments for all connected links
  //===============================================
  xm = []; ym = []; jj=0 ; //** init ...

  gh_link_i = []; gh_link_mod = [];

  connected = unique(get_connected(scs_m,k))//** look for connected link(s)

  //** ----------------------------------------------------------------------

  //** look for all the connected link(s) and build "impiling" the two data structures
  //** [xm , ym] for the links data points
  //** gh_link_i fro the associated graphic handles
  for l=1:length(connected)
    i  = connected(l) ;

    oi = scs_m.objs(i) ;

    //gh_i = o_size(1) - i + 1 ; //** calc the handler of all the connected link(s)
    gh_i=get_gri(i,o_size(1))

    gh_link_i = [ gh_link_i gh_curwin.children.children(gh_i) ]; //**

    [xl , yl,ct,from,to] = (oi.xx,oi.yy,oi.ct,oi.from,oi.to)

    nl = prod(size(xl)) ;
    if from(1)==k then
      xm = [xm,[xl(2);xl(1)]];
      ym = [ym,[yl(2);yl(1)]];
      // draw_link_seg(oi,1:2) //erase link
      // clr=[clr ct(1)]
    end

    if to(1)==k then
      xm = [xm,xl($-1:$)];
      ym = [ym,yl($-1:$)];
      // draw_link_seg(oi,$-1:$) //erase link
      // clr=[clr ct(1)]
    end

  end //** end of the for() loop
  //** ----------------------------------------------------------------------

  xmt = xm ; ymt = ym ; //** init ...

  [xmin, ymin] = getorigin(o) ; //** acquire the block origin coordinate
  dx = xc - xmin ; dy = yc - ymin ; //** calc the differential
  //** cursor_coordinate@the_user_press - coordinate of the object orgin
  //** this variable is used to move the object leaveng the cursor in the original
  //** position of the user press. At the end of the move [dx dy] are used to offset
  //** the cursor coordinate to obtain the block orgin coordinate and use this values
  //** to correcly update the scs_m datastructure 

  // move a block and connected links
  //=================================


  if size(connected,2)>0 then // move a block and connected links

    //** ------------- MOVE BLOCK AND CONNECTED LINKS LINKS --------------
    if %scicos_debug_gr then
      disp("do_stupidmove.sci : Move block WITH connected links ");
    end

    xco = xc; yco = yc;
    [xy,sz] = (o.graphics.orig, o.graphics.sz)

    //**-------------------------------------------------------------------
    gh_link_mod = [] ;
    tmp_data = [] ;
    t_xmt = [] ; t_ymt  = [];


    drawlater();

    while 1 do //** interactive move loop

      //rep = xgetmouse([%t,%t]);
      rep = xgetmouse(0,[%t,%t]);

      if or(rep(3)==[0,2,3,5,-5]) then
          break ; //** ---> EXIT point of the while
      end

      //** gh_curwin.figure_id
      //** if xget('window')<>curwin | rep(3)==-100 then
      gh_figure = gcf();
      if gh_figure.figure_id<>curwin | rep(3)==-100 then
	[%win,Cmenu] = resume(curwin,'Quit') ;
      end

      //**------------------------------------------------------------------
      //** Mouse movement limitation 
       if rep(1)>x_min & rep(1)<x_max
           delta_x = rep(1) - xc ; //** calc the differential position ...
	   xc = rep(1);
       else
           delta_x = 0.0 ;
       end 

       if rep(2)>y_min & rep(2)<y_max
           delta_y = rep(2) - yc ; //** calc the differential position ...
	   yc = rep(2) 
       else
           delta_y = 0.0 ;
       end
      //**------------------------------------------------------------------

	     move (gh_blk , [delta_x , delta_y]);  //** ..because "move()" works only in differential
	     //** xc = rep(1); yc = rep(2)

             xmt(2,:) = xm(2,:) - xco + xc ; //** update datas of links
             ymt(2,:) = ym(2,:) - yco + yc ;

	     j = 0 ; //** init
             //**---------------------------------------
             for l=1:length(connected) // ... for all the connected links 
	        i  = connected(l)  ; // from the progressive index "l" to the scs_m index "i"
	        oi = scs_m.objs(i) ; // get the "i"th link
	        [xl,from,to] = (oi.xx,oi.from,oi.to); // extract the proprieties from the link

		gh_link_mod = gh_link_i(l) ; // get the link graphics data structure

		if from(1)==k then
	            tmp_data = gh_link_mod.children.data ; // extract the vectors that define the link
		    // the first two points
		    j = j + 1 ; // update the [x,y]mt pointer
 		    t_xmt = xmt([2,1],j) ;  t_ymt = ymt([2,1],j) ; // estract the element
		    //** update the graphics datastructure
		    gh_link_mod.children.data = [ [t_xmt(1) , t_ymt(1)] ; tmp_data(2:$ , 1:$) ]  ;

		 end
                //** see the above comments :)
	        if to(1)==k then
		    tmp_data = gh_link_mod.children.data ;
		    // the last two points
		    j = j + 1 ;
    	            gh_link_mod.children.data = [ tmp_data(1:$-2 , 1:$) ; [xmt(:,j) , ymt(:,j)] ]  ;
		end
	      end // ... of for() link loop

               draw(gh_blk.parent);
               show_pixmap();

    end //** ...  of while
    //**-------------------------------------------------------------------

    gh_figure = gcf();
    if gh_figure.figure_id<>curwin | rep(3)==-100 then
         [%win,Cmenu] = resume(curwin,'Quit') ;
    end

    // update and block and links position in scs_m
    if and(rep(3)<>[2 5]) then
      xy = [xc-dx,yc-dy];
      o.graphics.orig = xy;
      scs_m.objs(k) = o; //update block coordinates

      j = 0 ; //** init
      //**---------------------------------------
      for l=1:length(connected)
        i  = connected(l)  ;
        oi = scs_m.objs(i) ;
        [xl,from,to] = (oi.xx,oi.from,oi.to);

        if from(1)==k then
          j = j + 1 ;
          oi.xx(1:2) = xmt([2,1],j)
          oi.yy(1:2) = ymt([2,1],j)
          // draw_link_seg(oi,1:2) //draw link
        end

        if to(1)==k then
          j = j+1
          oi.xx($-1:$) = xmt(:,j)
          oi.yy($-1:$) = ymt(:,j)
          // draw_link_seg(oi,$-1:$) //draw link
        end
        scs_m.objs(i) = oi ;
      end //... for loop
    else //*restore original position of block and links in figure
      drawlater();
      move (gh_blk , xy(:)'-[xc-dx,yc-dy]);  //** ..because "move()" works only in differential
      xmt(2,:) = xm(2,:); //** original datas of links
      ymt(2,:) = ym(2,:);
      j = 0 ; //** init
      //**---------------------------------------
      for l=1:length(connected)
        i  = connected(l)  ;
        oi = scs_m.objs(i) ;
        [xl,from,to] = (oi.xx,oi.from,oi.to);
        gh_link_mod = gh_link_i(l) ; // get the link graphics data structure

        if from(1)==k then
          tmp_data = gh_link_mod.children.data ;
          j = j + 1 ;
          t_xmt = xmt([2,1],j) ;  t_ymt = ymt([2,1],j) ;
          gh_link_mod.children.data = [ [t_xmt(1) , t_ymt(1)] ; tmp_data(2:$ , 1:$) ];
        end

        if to(1)==k then
          tmp_data = gh_link_mod.children.data ;
          j = j+  1 ;
          gh_link_mod.children.data = [ tmp_data(1:$-2 , 1:$) ; [xmt(:,j) , ymt(:,j)] ];
        end
      end //... for loop
      draw(gh_curwin.children);
      show_pixmap();

    end
      //**----------------------------------------

  //** ---------------------------- MOVE BLOCK W/O LINKS ------------------------
  else // move an unconnected block
    if %scicos_debug_gr then
      disp("do_stupidmove.sci : Move block without links");
    end

    [xy,sz] = (o.graphics.orig,o.graphics.sz)

    //**----------------------------------------------------------------------
    // %xc = xy(1); %yc = xy(2); //** default start position 
    drawlater();
    while 1 do

      rep = xgetmouse(0,[%t,%t]); // get new position

      if or(rep(3)==[0, 2, 3, 5, -5, -100]) then break ; end //** ---> EXIT point of the while
      //**------------------------------------------------------------------
      //** Mouse movement limitation 
       if rep(1)>x_min & rep(1)<x_max 
           delta_x = rep(1) - xc ; //** calc the differential position ...
	   xc = rep(1);
       else
           delta_x = 0.0 ;
       end
       if rep(2)>y_min & rep(2)<y_max
           delta_y = rep(2) - yc ; //** calc the differential position ...
	   yc = rep(2) 
       else
           delta_y = 0.0 ;
       end
      //**------------------------------------------------------------------

      move (gh_blk , [delta_x , delta_y]);  //** ..because "move()" works only in differential
      draw(gh_curwin.children);
      show_pixmap();

    end
    //**---------------------------------------------------------------------

    gh_figure = gcf();
    if gh_figure.figure_id<>curwin | rep(3)==-100 then
	[%win,Cmenu] = resume(curwin,'Quit') ;
    end

    // Update block coordinates
    if and(rep(3)<>[2 5]) then
        //update block coordinates
	xy = [xc - dx, yc - dy];
        o.graphics.orig = xy;
        scs_m.objs(k) = o;
    else //*restore original position of block
        drawlater();
        move (gh_blk , xy-[xc-dx,yc-dy]);  //** ..because "move()" works only in differential
        draw(gh_curwin.children);
        show_pixmap();
    end

  end // end of Move Block with / without Link(s)

endfunction
//**--------------------------------------------------------------------------

//** ----------------------> This function works only with links <----------------------
//** ---------- Link Supid Move ---------------------
function scs_m = stupid_movecorner(scs_m, k, xc, yc, wh)
  
  //**----------------------------------------------------------------------------------
  //** the code below is modified according the new graphics API
  gh_curwin = gh_current_window ; 
  
  //**-----------------------------------------------------------------------------------
  //** Acquire axes phisical limits (visible limits are smaller)    
     
  figure_axes_size = gh_curwin.axes_size ; //** size in pixel 
  x_f = figure_axes_size(1) ;
  y_f = figure_axes_size(2) ;
  
  [x1_f, y1_f, rect_f] = xchange([0,x_f],[0,y_f],"i2f"); //** convert to local coordinate 
  
  x_min = x1_f(1) ; x_max = x1_f(2) ; //** hor. limits
  y_min = y1_f(2) ; y_max = y1_f(1) ; //** ver. limits (inverted because the upper left corner effect)  
    
  //**-----------------------------------------------------------------------------------
  
  //** at this point I need to build the [scs_m] <-> [gh_window] datastructure 
  //** I need an equivalent index for the graphics 
  o_size = size (gh_curwin.children.children ) ; //** the size:number of all the object 
  
  //** "k" is the object index in the data structure "scs_m"
  //** compute the equivalent "gh_k" for the graphics datastructure 
  //gh_k = o_size(1) - k + 1 ; //** semi empirical equation :) //** Updated by Alan 
  gh_k = get_gri(k,o_size(1))
  //** disp (gh_k);
  gh_blk = gh_curwin.children.children(gh_k);
  
  //**-----------------------------------------------------------------------------------
  
  o = scs_m.objs(k) ; //** take the Scicos object 
  
  [xx, yy, ct] = (o.xx,o.yy,o.ct) ;
  //** ct(1) is the color ; never used because it is not modified :)
  
  o_link = size (xx)    ;
  link_size = o_link(1) ; //** number of element of link (polyline) 

  //** wh point to the subsegment (sublink interested by the move) 
  moving_seg = [-wh-1:-wh+1] ;
  
  if (-wh-1) == 1 then //** the moving include the starting point  
    start_seg = [];
    X_start = []  ;
    Y_start = []  ;
  else                 //** the move need some static point from the beginning 
    start_seg = [1 : -wh-1]  ; 
    X_start = xx(start_seg)  ;
    Y_start = yy(start_seg)  ;
  end    
  
  if (-wh+1) == link_size then //** the moving include the endpoint 
    end_seg = [] ;
    X_end = []  ;
    Y_end = []  ;
  else                //** the moving need some static point to the end 
    end_seg = [-wh+1 : link_size] ;
    X_end = xx(end_seg) ;
    Y_end = yy(end_seg) ;
  end   
      
  if %scicos_debug_gr then
    disp("...subsegment -> Link move --> seg:..."); disp(moving_seg) ;
  end

  //** xpolys(xx(seg),yy(seg),ct(1)) //draw thin link
  //** gh_blk.children.data = [xx(seg), yy(seg)]; //** modify the fixed part of the link 

  X1 = xx(moving_seg) ; //** constant starting values
  Y1 = yy(moving_seg) ;
  x1 = X1 ;  //** temp variables for the move
  y1 = Y1 ;

  //** store initial gh_blk.children.data
  //**  -Alan- YAUNTOUTPETITBUGICI
  //** gh_blk.children.data peut ne pas etre en accord avec scs_m
  //** a' cause de la fonction stupid_getobj qui dans le cas
  //** des liens peut creer un nouveau point
  ini_data = gh_blk.children.data;

  //**-----------------------------------------------------------------
  rep(3) = -1;
  drawlater();
  while 1 do
    if or(rep(3)==[0,2,3,5,-5,-100]) then break,end ; //** exit point

    gh_blk.children.data = [X_start,Y_start ; x1, y1 ; X_end, Y_end ];
    draw(gh_curwin.children);
    show_pixmap();

    rep = xgetmouse(0,[%t,%t]);

    gh_figure = gcf();
    if gh_figure.figure_id<>curwin | rep(3)==-100 then
         [%win,Cmenu] = resume(curwin,'Quit') ;
    end

    //**------------------------------------------------------------------
      //** Mouse movement limitation
       if rep(1)>x_min & rep(1)<x_max
	   xc1 = rep(1);
       end

       if rep(2)>y_min & rep(2)<y_max
	   yc1 = rep(2) 
       end
    //**------------------------------------------------------------------

    x1(2) = X1(2) - (xc - xc1) ;
    y1(2) = Y1(2) - (yc - yc1) ;
  end //** of the while "interactive" loop  
  //**------------------------------------------------------------------

  gh_figure = gcf();
  if gh_figure.figure_id<>curwin | rep(3)==-100 then
      [%win,Cmenu] = resume(curwin,'Quit') ;
  end

  if %scicos_debug_gr then
    disp("...subsegment -> Link move --> rep(3):..."); disp(rep(3)) ;
  end

  if and(rep(3)<>[2 5]) then //** if the link manipulation is OK 
    if abs(x1(1)-x1(2))<rela*abs(y1(1)-y1(2)) then
      x1(2)=x1(1)
    elseif abs(x1(2)-x1(3))<rela*abs(y1(2)-y1(3)) then
      x1(2)=x1(3)
    end  
    if abs(y1(1)-y1(2))<rela*abs(x1(1)-x1(2)) then
      y1(2)=y1(1)
    elseif abs(y1(2)-y1(3))<rela*abs(x1(2)-x1(3)) then
      y1(2)=y1(3)
    end  
    d = projaff([x1(1);x1(3)],[y1(1);y1(3)],[x1(2);y1(2)])
    if norm(d(:)-[x1(2);y1(2)])<..
	  rela*max(norm(d(:)-[x1(3);y1(3)]),norm(d(:)-[x1(1);y1(1)])) then
      xx(moving_seg)=x1
      yy(moving_seg)=y1
      xx(moving_seg(2))=[]
      yy(moving_seg(2))=[]
      x1(2)=[];y1(2)=[];moving_seg(3)=[]
    else
      xx(moving_seg)=x1
      yy(moving_seg)=y1
    end
    
    //** update the graphics object with the last link geometrical correction 
    drawlater();
     gh_blk.children.data = [xx , yy];
     draw(gh_blk.parent);
    show_pixmap();
    
    o.xx = xx; o.yy = yy ;
    scs_m.objs(k)=o ; //** update the scs_m data structure

  else //** restore original position of link in figure
    
    drawlater();
     gh_blk.children.data = ini_data;
     draw(gh_blk.parent);
    show_pixmap();
    
    //** DO NOT update the scs_m datastructure ! ;)
  
  end

endfunction
