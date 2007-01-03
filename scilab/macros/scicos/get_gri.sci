function gr_k=get_gri(k,l)
  //** 03/01/07 : Alan
  //** semi empirical equation :)
  //** before : gr_k = l - k + 1,
  //** but now there is always a grid
  //** in the last children
  gr_k = l - k ;
endfunction
