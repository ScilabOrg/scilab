function [k,wh,scs_m] = stupid_getobj(scs_m,pt)
//** INRIA
//**-----------------------------------------------------------------------------------------------
  
  n = lstsize(scs_m.objs)
  wh = [];
  x = pt(1); y = pt(2) ; //** mouse coordinate 
  data = [] ;
  k = [];

  //** -------------------- Loop for every object --------------------------------------
  for i=1:n //loop on objects

    o = scs_m.objs(i);

    //**-------- If it is a Block ---------------------------------------
    if typeof(o)=='Block' then
      graphics = o.graphics
      [orig,sz] = (graphics.orig,graphics.sz)
      data = [(orig(1)-x)*(orig(1)+sz(1)-x),(orig(2)-y)*(orig(2)+sz(2)-y)]
      if data(1)<0&data(2)<0 then k=i, break, end ;

    //**--------- If it is a Link ----------------------------------------
    elseif typeof(o)=='Link' then
      eps = 3 ;
      xx = o.xx; yy = o.yy;
      [d, ptp, ind] = stupid_dist2polyline(xx, yy, pt, 0.85)

      if d<eps then 
	
	if ind==-1 then 
	  k = o.from(1); //** click near an input
	  break ;
	elseif ind==-size(xx,1) then 
	  k = o.to(1) ; //** click near an output
	  break ;
	elseif ind>0 then 
	  // draw_link_seg(o,[ind,ind+1]) //** debug only
	  o.xx = [xx(1:ind);ptp(1);xx(ind+1:$)];
	  o.yy = [yy(1:ind);ptp(2);yy(ind+1:$)];
	  scs_m.objs(i) = o ; //** CESTICIQUONCREEUNPOINTDANSSCS_M
                              //** ETQUONOUBLIEDELEFAIREDANSLEGRAPHIQUE !!
	  k  = i ;
	  wh = -ind - 1 ; //** click in the middle (case 1) of a link
	  break ;
	else
	  k = i
	  wh = ind      ; //** click in the middle (case 2) of a link
	  // draw_link_seg(o,[-ind-1:-ind+1]); //** debug only
	  break ;
	end
      end

    //**---------- If it is a Text --------------------------------------
    elseif typeof(o)=='Text' then
      graphics=o.graphics
      [orig,sz]=(graphics.orig,graphics.sz)
      data=[(orig(1)-x)*(orig(1)+sz(1)-x),(orig(2)-y)*(orig(2)+sz(2)-y)]
      if data(1)<0&data(2)<0 then k=i,break,end
    end
  end
endfunction
