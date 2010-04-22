// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// neldermead_display --
//   Display a Nelder-Mead engine
//
function neldermead_display ( this )
  warning ( msprintf ( gettext ( "%s: obsolete use of neldermead_display, use directly the variable instead."), "neldermead_display" ) )
  str = string ( this )
  srows = size(str,"r")
  for i = 1 : srows
    mprintf("%s\n",str(i))
  end
endfunction

  //
  // _strvec --
  //  Returns a string for the given vector.
  //
  function str = _strvec ( x )
      str = strcat(string(x)," ")
  endfunction

