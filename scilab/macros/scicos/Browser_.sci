function Browser_()
Cmenu=[]
if super_path==[] then
  disablemenus()
  do_browser(scs_m);
  enablemenus() 
else
  message('Browser can only be launched from main diagram.')
end
endfunction
