function scs_m=update_redraw_obj(scs_m,path,o)
// Copyright INRIA
//** 02/12/06 : use of objects permutation in gh_curwin.children.children()

if length(path)==2 then
  if typeof(o)=='Link'|typeof(o)=='Text' then
    scs_m(path)=o
    //quick update for new graphics
    drawlater() ;
    gh_curwin = gh_current_window;
    o_size = size(gh_curwin.children.children);
    gr_k = o_size(1) - path(2) + 1; //** semi empirical equation :)
    update_gr(gr_k,o) //** new
    draw(gh_curwin.children);
    show_pixmap() ;
  else
    //new graphics functions is now included in changeports
    scs_m = changeports(scs_m,path,o)
  end
else // change a block in a sub-level

  scs_m(path) = o ;

end

endfunction
