function [%pt,scs_m] = do_resize(%pt,scs_m)
//
// Copyright INRIA
//**
//** 6 Sett 2006: Simone Mannori
//** Change the size (w,h) of a Block
//** 02/12/06-@l@n- : use of objects permutation in
//**                  gh_curwin.children.children()

  win = %win;

  //** Fiter out the multiple object selected / not in current window cases
  if Select==[] then //** if no object is already selected
    xc  = %pt(1); //** acquire the last "cosclick" position and
    yc  = %pt(2);
    %pt = []    ; //** look for a object
    K = getblocklink(scs_m,[xc;yc]) ;
  else
    K   = Select(:,1)';
    %pt = [] ;
    if size(K,'*')>1 | %win<>Select(1,2) then
      message("Only one block can be selected in current window for this operation.")
      Cmenu=[]; %pt=[]; return ; //** ---> EXIT
    end
  end

  //** filter the object type
  if K<>[] then

    if typeof(scs_m.objs(K))=='Block' then
    //** ----- Block ------------------------------------------
      o = scs_m.objs(K) ;
      graphics = o.graphics    ;
      sz       = graphics.sz   ;
      orig     = graphics.orig ;

      [ok,w,h] = getvalue('Set Block sizes',['width';'height'],..
	  	           list('vec',1,'vec',1),string(sz(:)))
      //** in case of valid (w,h)
      if ok  then
         w = maxi(w,5) ;
         h = maxi(h,5) ;

         if w<>sz(1) then
           if [get_connected(scs_m,K,'out'),..
	       get_connected(scs_m,K,'clkin'),..
	       get_connected(scs_m,K,'clkout')]<>[] then
	       hilite_obj(K)
               message(['Block with connected standard port outputs'
                        'or Event ports cannot be resized horizontally'])
	       unhilite_obj(K)
               return ; //** exit from the function
           end
         end

         if h<>sz(2) then
           if [get_connected(scs_m,K,'out'),..
	       get_connected(scs_m,K,'in'),..
	       get_connected(scs_m,K,'clkin')]<>[] then
	       hilite_obj(K)
               message(['Block with connected standards ports'
                        'or Event input ports cannot be resized vertically'])
	       unhilite_obj(K)
              return ; //** exit from the function
           end
         end

         graphics.sz   = [w;h] ;
         graphics.orig = orig  ;
         o.graphics    = graphics ;
         scs_m.objs(K) = o  ;

         drawlater() ;
         gh_curwin = gh_current_window;
         o_size = size(gh_curwin.children.children);
         gr_k = o_size(1) - K + 1; //** semi empirical equation :)

         update_gr(gr_k,o);
         draw(gh_curwin.children);
         show_pixmap() ;

      end //** of ok

    elseif typeof(scs_m.objs(K))=='Link' then
    //** it is a Link
      //** extract some link parameter
      [pos,ct] = (scs_m.objs(K).thick, scs_m.objs(K).ct) ;
      Thick = pos(1) ;
      Type  = pos(2) ;
      //** open a dialog box to edit
      [ok,Thick,Type] = getvalue('Link parameters',['Thickness';'Type'],..
			          list('vec','1','vec',1),[string(Thick);string(Type)])
      if ok then
	edited = or(scs_m.objs(K).thick<>[Thick,Type]) ; //** set flag if the parm are edited
	scs_m.objs(K).thick = [Thick,Type]             ; //** update the 'scs_m' data structure
        //** quick update for new graphics
        //<<<<<<<<<<<<<<<<<
        drawlater() ;
        gh_curwin = gh_current_window;
        o_size = size(gh_curwin.children.children);
        gr_k = o_size(1) - K + 1; //** semi empirical equation :)
        //This is done in accord to drawobj
        gh_curwin.children.children(gr_k).children(1).thickness = ...
                             maxi( scs_m.objs(K).thick(1) , 1) * ...
                             maxi(scs_m.objs(K).thick(2), 1) ;
        draw(gh_curwin.children);
        show_pixmap() ;
        //>>>>>>>>>>>>>>>>>>>
      end

    else
    //** nor Block nor Link
      message("Resize is allowed only for Blocks or Links.")
    end

  else
    //** no valid object is found then ...
    Cmenu=[];

  end //** of "I found an object"

endfunction
