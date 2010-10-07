// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_tclsci_scroll()

  consolestatus = 'off';
  tkpath = SCI + "/modules/tclsci/demos/tk/";

  TCL_EvalFile(tkpath+'cscroll')

  if getos() == 'Windows' then
    consolestatus = consolebox();
    if consolestatus == 'off' then
      consolebox("on");
    end
  end

  while %t //wait for toplevel to disapear
    TCL_EvalStr('set h [winfo exists .cscroll]');
    if TCL_GetVar("h")=='0' then break,end
    sleep(1);
  end

  if getos() == 'Windows' then
    consolebox(consolestatus);
  end

endfunction

demo_tclsci_scroll();
clear demo_tclsci_scroll;
