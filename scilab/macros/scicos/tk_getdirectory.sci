function path=tk_getdirectory()
  TCL_EvalStr("set dirdd [tk_chooseDirectory]")
  path=stripblanks(TCL_GetVar('dirdd'))
  if path==emptystr() then path=[],end
endfunction                           