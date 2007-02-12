function gr_k=get_gri(k,l)
  //** 03/01/07 : Alan
  //** semi empirical equation :)
  //
  // input : k the index of the object in the
  //         not compiled scicos structure 
  //        (scs_m.objs)
  //
  //        l the total number of graphic objects
  //        in the current graphical structure
  //        size(gh.children.children)
  //
  // output : gr_k the corresponding index of the
  //          not compiled element in the current
  //          graphical structure
  //
  //** before : gr_k = l - k + 1,
  //** but now there is always a grid
  //** in the last children
  gr_k = l - k ;
endfunction
