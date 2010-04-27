// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimsimplex_print --
//   Display the current simplex, with coordinates and function values.
// Arguments
//   <no arg>
//
function optimsimplex_print ( this )
  warning ( msprintf ( gettext ( "%s: obsolete use of optimsimplex_print, use directly the variable instead."), "optimsimplex_print" ) )
  str = string ( this )
  srows = size(str,"r")
  for i = 1 : srows
    mprintf("%s\n",str(i))
  end
endfunction

