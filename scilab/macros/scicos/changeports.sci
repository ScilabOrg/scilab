function scs_m = changeports(scs_m, path, o_n)
//** INRIA 
//**
//** 27 July 2007
//**
//
//**         BEWARE: this version has some residual graphical bug with some 
//**                   Modelica electrical connected blocks
//* 
//
//
//**         BEWARE : This is an ---> EXPERIMENTAL VERSION <--- ! 
//**
//** This function is able to replace any block with any other block. We made a reasonable effort 
//** to reutilize the information associated at the ports with the aim to mantain the connections. 
//** When the connection are incompatible, the links are removed.
//** The existing links are moved in order to match the port's positions.
//** 
//** Please left the disp messages: they can be useful as debug utility if someone will add more
//** port type. 
//**
//**
//** ToDo : adjust the links in square angle (horizontal and vertical links only).
//**
//**

  //** The very first time the this routine try to match the ports of the new blocks over the ports
  //** of the old one.
  //** "LinkToDel" is a vector of "scs_m" index of connected links of the old blocks that cannot be
  //** relocated on the new one and must be deleted  
   
  [scs_m, o_n, LinkToDel] = match_ports(scs_m, path, o_n);
  
  //**---------- Delete the links relative at the unconnected (non allocated) ports --------------
   
  for i=1:size(LinkToDel,'*')
     Link_index = LinkToDel(i) ; //** the Link to be deleted 
     gr = %t                   ; //** update the screen   
     [scs_m, DEL, DELL] = do_delete1(scs_m, Link_index, gr) ; //** delete the links 
  end 
  
  //**--------------------------------------------------------------------------------------------- 
  
  //** The deletion of some links can create new unconnected ports on the original block:
  //** the same routine is reused  
  
  [scs_m, o_n, LinkToDel] = match_ports(scs_m, path, o_n)
  
  //** Now "LinkToDel" must be empty : this is just a warning code 
  if LinkToDel<>[] then 
    disp("LinkToDel must be empty on the second pass");
  end 
   
  //** ------- Update  block --------------------------------------------

if %diagram_open then
  //** ------- Graphics ---------------
  drawlater() ;
  gh_curwin = gh_current_window;
  o_size = size(gh_curwin.children.children);
  
  k = path($) ; //** the scs_m index of the target 
  gr_k = get_gri(k, o_size(1))
  
  //** redraw block
  //** quick update for new graphics
  update_gr(gr_k, o_n)    ;
  draw(gh_curwin.children); //** redraw the graphic data structure 
  show_pixmap();
  
end
  //**-------- Scicos -----------------
  //** update block in scicos structure
  
  scs_m.objs(k) = o_n ;

endfunction
//**-------------------------------------------------------------------------------------------------
//**
//**-------------------------------------------------------------------------------------------------

function [scs_m, o_n, LinkToDel] = match_ports(scs_m, path, o_n)
//** ---- INITIALIZATION ----
  //**
  //**
    
  //** isolate the object that will be substituited 
  o = scs_m(path) ; 
  
  //** extract the proprieties of the OLD object 
  [pin, pout, pein, peout, in_mod, out_mod] = (o.graphics.pin,  o.graphics.pout, ...
                                               o.graphics.pein, o.graphics.peout, ...
                                               o.graphics.in_implicit, o.graphics.out_implicit);
  
  //** "o_n" is the NEW object 
  [pin_n, pout_n, pein_n, peout_n, in_mod_n, out_mod_n] = (o_n.graphics.pin,  o_n.graphics.pout, ...
                                                           o_n.graphics.pein, o_n.graphics.peout, ...
							   o_n.graphics.in_implicit, o_n.graphics.out_implicit);
  //** ---------------------------------------------
  //**
  
  //** ------------ QUICK & DIRTY TRICK -------------
  //** This code "export back" the "E" (explicit, standard) / "I" (implicit, Modelica)
  //** proprieties to the "old" block because very old block/superblock does not have 
  //** these proprieties 
  
  //** for the "old blocks" input(s) with not defined proprieties  
  if in_mod==[] then 
       in_mod = in_mod_n ; //** "export back" the "E" / "I" 
  end 
  
  //** for the "old blocks" output(s) with not defined proprieties  
  if out_mod==[] then
      out_mod = out_mod_n ; //** "export back" the "E" / "I"
  end 
  
  //**-----------------------------------------------
  
  
  //** acquire the dimension of the new block input and output ports and
  //** put all the ports of the new block in unconnected [0] state  
    
  size_pin_n = size(pin_n,'*')  ;
  pin_n = zeros(size_pin_n, 1 ) ; 
    
  size_pout_n = size(pout_n,'*') ;  
  pout_n = zeros(size_pout_n, 1) ;
  
  size_pein_n = size(pein_n,'*') ;
  pein_n = zeros(size_pein_n, 1) ;
  
  size_peout_n = size(peout_n,'*') ;
  peout_n = zeros(size_peout_n, 1) ; 
  
  //** Acquire the size of the old block
  size_pin   = size(pin,'*')   ;
  size_pout  = size(pout,'*')  ;  
  size_pein  = size(pein,'*')  ;
  size_peout = size(peout,'*') ;
  

  //** New object ports position and type 
  //** x , y : absolute coordinates of the port 
  //** t     : 1 (standard); -1 (event) ; 2 (Modelica)
  execstr('[xin, yin, tin]='+o.gui+'(''getinputs'', o)');
  execstr('[xin_n, yin_n, tin_n]='+o.gui+'(''getinputs'', o_n)');
  
  execstr('[xout, yout, tout]='+o.gui+'(''getoutputs'',o)');
  execstr('[xout_n, yout_n, tout_n]='+o.gui+'(''getoutputs'',o_n)');
  
  size_inputs   = size(tin,'*')     ;
  size_inputs_n = size(tin_n,'*')   ;
  size_outputs    = size(tout,'*')   ;
  size_outputs_n  = size(tout_n,'*') ;
  
  //** Inputs of the OLD block 
  xsmin=[] ; ysmin =[] ; tsmin = []; xein = [] ; yein = [] 
  for i=1:size_inputs
  
     if tin(i)==1 | tin(i)==2 then //** standard OR Modelica input 
        xsmin = [ xsmin xin(i) ] ;
	ysmin = [ ysmin yin(i) ] ;
	tsmin = [ tsmin tin(i) ] ;
     elseif tin(i)==-1 then //** event input 
        xein = [ xein xin(i) ] ;
	yein = [ yein yin(i) ] ;
     else
       disp("InOLD:The input type is unknow"); pause 
     end    
  
  end //**... of for loop 
  
  //** Inputs of the NEW block 
  xsmin_n=[] ; ysmin_n =[] ; tsmin_n = []; xein_n = [] ; yein_n = [] 
  for i=1:size_inputs_n
  
     if tin_n(i)==1 | tin_n(i)==2 then //** standard OR Modelica input 
        xsmin_n = [ xsmin_n xin_n(i) ] ;
	ysmin_n = [ ysmin_n yin_n(i) ] ;
	tsmin_n = [ tsmin_n tin_n(i) ] ;
     elseif tin_n(i)==-1 then //** event input 
        xein_n = [ xein_n xin_n(i) ] ;
	yein_n = [ yein_n yin_n(i) ] ;
     else
       disp("InNew:The input type is unknow"); pause 
     end    
  
  end //**... of for loop 

  //** Outputs of the OLD block 
  xsmout=[] ; ysmout =[] ; tsmout = []; xeout = [] ; yeout = [] 
  for i=1:size_outputs
  
     if tout(i)==1 | tout(i)==2 then //** standard OR Modelica output 
        xsmout = [ xsmout xout(i) ] ;
	ysmout = [ ysmout yout(i) ] ;
	tsmout = [ tsmout tout(i) ] ;
     elseif tout(i)==-1 then //** event input 
        xeout = [ xeout xout(i) ] ;
	yeout = [ yeout yout(i) ] ;
     else
       disp("OutOLD:The output type is unknow"); pause 
     end    
  
  end //**... of for loop 
  
  //** Outputs of the NEW block 
  xsmout_n=[] ; ysmout_n =[] ; tsmout_n = []; xeout_n = [] ; yeout_n = [] 
  for i=1:size_outputs_n
  
     if tout_n(i)==1 | tout_n(i)==2 then //** standard OR Modelica output 
        xsmout_n = [ xsmout_n xout_n(i) ] ;
	ysmout_n = [ ysmout_n yout_n(i) ] ;
	tsmout_n = [ tsmout_n tout_n(i) ] ;
     elseif tout_n(i)==-1 then //** event input 
        xeout_n = [ xeout_n xout_n(i) ] ;
	yeout_n = [ yeout_n yout_n(i) ] ;
     else
       disp("OutNew:The output type is unknow"); pause 
     end    
  
  end //**... of for loop

  InputLinkToCon  = []; xInPortToCon  = [] ;  yInPortToCon  = [] ;
  OutputLinkToCon = []; xOutPortToCon = [] ;  yOutPortToCon = [] ; 
  
  LinkToDel = [];
  
  //** disp("...changeports... : inspect variables"); pause //** DEBUG ONLY 
  
  //**------------------ INPUT PORTS ---------------------------------------------------------------
  if size_pin >0 then 
   
    for i=1:size_pin //** for all the input "pin" of the old block 
  //** if the port is linked AND the new block has enough ports AND the two ports are of the same E/I 
    //**                                                    AND the same Standard/Modelica type  
       if  pin(i)>0 & i<=size_pin_n & in_mod(i)==in_mod_n(i) & tsmin(i)==tsmin_n(i) then  
         pin_n(i) = pin(i); //** assign the port to the old Link 
	 InputLinkToCon = [InputLinkToCon pin(i)] ;   //** add the Link to the "to be reconnected links" vector
           xInPortToCon = [xInPortToCon xsmin_n(i)] ; //** recover the coordinate of the new equivalent port 
	   yInPortToCon = [yInPortToCon ysmin_n(i)] ; //** and pile up in the vector 
       else
       	 if pin(i)>0 //** if the old port was connected 
	   LinkToDel = [LinkToDel pin(i)]; //** add the Link to the "to be deleted links" vector 
	 end 
       end 
    
    end //** of the for loop 
    
    o_n.graphics.pin = pin_n ; //** update the "scs_m" input_port - link association  
    
  end 
  //**---------------------------------------------------------------------------------------------
  
  //**------------------ EVENT INPUT PORTS --------------------------------------------------------
  if size_pein >0 then 
    
    for i=1:size_pein //** for all the event input of the old block 
  //** if the port is linked AND the new block has enough ports  
       if  pein(i)>0 & i<=size_pein_n then  
         pein_n(i) = pein(i); //** assign the port to the old Link
         InputLinkToCon = [InputLinkToCon pein(i)] ; //** add the Link to the "to be reconnected links" vector
	   xInPortToCon = [xInPortToCon xein_n(i)] ; //** recover the coordinate of the new equivalent port 
	   yInPortToCon = [yInPortToCon yein_n(i)] ; //** and pile up in the vector 
       else
       	 if pein(i)>0 //** if the old port was connected 
	   LinkToDel = [LinkToDel pein(i)]; //** add the Link to the "to be deleted links" vector 
	 end 
       end 
    
    end //** of the for loop 
  
    o_n.graphics.pein = pein_n ; //** update the "scs_m" event_input_port - event_link association
    
  end 
  //**---------------------------------------------------------------------------------------------

  //**------------------ OUTPUT PORTS -------------------------------------------------------------
  if size_pout >0 then 
    
    for i=1:size_pout //** for all the output "pout" of the old block 
  //** if the port is linked AND the new block has enough ports AND the two ports are of the same E/I 
    //**                                                        AND the same Standard/Modelica type    
       if  pout(i)>0 & i<=size_pout_n & out_mod(i)==out_mod_n(i) & tsmout(i)==tsmout_n(i) then  
         pout_n(i) = pout(i); //** assign the port to the old Link
	 OutputLinkToCon = [OutputLinkToCon pout(i)] ; //** add the Link to the "to be reconnected links" vector
           xOutPortToCon = [xOutPortToCon xsmout_n(i)] ; //** recover the coordinate of the new equivalent port 
	   yOutPortToCon = [yOutPortToCon ysmout_n(i)] ; //** and pile up in the vector  
       else
       	 if pout(i)>0 //** if the old port was connected 
	   LinkToDel = [LinkToDel pout(i)]; //** add the Link to the "to be deleted links" vector 
	 end 
       end 
    
    end //** of the for loop 
  
    o_n.graphics.pout = pout_n ; //** update the "scs_m" ouput_port - link association
    
  end 
  //**---------------------------------------------------------------------------------------------

  //**------------------ EVENT OUTPUT PORTS -------------------------------------------------------
  if size_peout >0 then 
    
    for i=1:size_peout //** for all the event input of the old block 
  //** if the port is linked AND the new block has enough ports   
       if  peout(i)>0 & i<=size_peout_n then  
         peout_n(i) = peout(i); //** assign the port to the old Link
	 OutputLinkToCon = [OutputLinkToCon peout(i)] ; //** add the Link to the "to be reconnected links" vector
	   xOutPortToCon = [xOutPortToCon xeout_n(i)] ; //** recover the coordinate of the new equivalent port 
	   yOutPortToCon = [yOutPortToCon yeout_n(i)] ; //** and pile up in the vector
       else
       	 if peout(i)>0 //** if the old port was connected 
	   LinkToDel = [LinkToDel peout(i)]; //** add the Link to the "to be deleted links" vector 
	 end 
       end 
    
    end //** of the for loop 
  
    o_n.graphics.peout = peout_n ; //** update the "scs_m" event_ouput_port - event_link association
    
  end 
  //**--------------------------------------------------------------------------------------------- 
if %diagram_open then
    
  //** New graphics section 
  drawlater() ;
  gh_curwin = gh_current_window;
  o_size = size(gh_curwin.children.children);
  
  k = path($) ; //** the scs_m index of the target 
  gr_k = get_gri(k, o_size(1))
  
  gh_link = [];
end
   
  //** ------------------------ ADJUST THE CONNECTED LINKS ----------------------------------------
  
  //**-------------------------- Input Links ---------------------------------------
  if size(InputLinkToCon,'*') > 0 then 
  
  //** disp("Adj links"); pause 
  
   for i=1:size(InputLinkToCon,'*')
  
      Link_index = InputLinkToCon(i) ;
      oi = scs_m.objs(Link_index)
      
      [xlink, ylink, ct ,from ,to ] = (oi.xx, oi.yy, oi.ct, oi.from, oi.to) ;
  
      xlink($) = xInPortToCon(i) ; ylink($) = yInPortToCon(i) ; //** force the position of last point
      
      //** Utilise the "theta" block parameters to compute the ppysical position of the port on the screen
      xxx = rotate([xlink($);ylink($)],...
                   o_n.graphics.theta*%pi/180,...
                  [o_n.graphics.orig(1)+o_n.graphics.sz(1)/2;o_n.graphics.orig(2)+o_n.graphics.sz(2)/2]);
      xlink($) = xxx(1,:);
      ylink($) = xxx(2,:);
      
      oi.xx = xlink ; oi.yy = ylink ;                           //** link 
      scs_m.objs(Link_index) = oi; //** update the scs_m 
      
if %diagram_open then
      ghi = get_gri(Link_index, o_size(1) );       //** calc the index of the connected link
      gh_link = gh_curwin.children.children(ghi);  //** recover the handle 
      gh_link.children.data = [oi.xx , oi.yy];//** update the object  
end      
   end //** for loop 
  
  end   
  
  //**---------------------------------
  
  //**----------------------- Output Links -------------------------------------------
  
  if size(OutputLinkToCon,'*') > 0 then 
  
   for i=1:size(OutputLinkToCon,'*')
      
      Link_index = OutputLinkToCon(i) ;
      oi = scs_m.objs(Link_index)
      
      [xlink, ylink, ct ,from ,to ] = (oi.xx, oi.yy, oi.ct, oi.from, oi.to) ;
  
      xlink(1) = xOutPortToCon(i) ; ylink(1) = yOutPortToCon(i) ; //** force the first point of the link 
      
      //** Utilise the "theta" block parameters to compute the physical position of the port on the screen
      xxx = rotate([xlink(1);ylink(1)],...
                   o_n.graphics.theta*%pi/180,...
                  [o_n.graphics.orig(1)+o_n.graphics.sz(1)/2;o_n.graphics.orig(2)+o_n.graphics.sz(2)/2]);
      xlink(1) = xxx(1,:);
      ylink(1) = xxx(2,:);
      
      oi.xx = xlink ; oi.yy = ylink ; 
      
      scs_m.objs(Link_index) = oi;    //** update the scs_m 
if %diagram_open then      
      ghi = get_gri(Link_index, o_size(1) );       //** calc the index of the connected link
      gh_link = gh_curwin.children.children(ghi);  //** recover the handle 
      gh_link.children.data = [oi.xx , oi.yy];//** update the object 
end
   end //** for loop  
  
  end  
  //** ------------------------ END OF : ADJUST THE CONNECTED LINKS -------------------------------  

endfunction





