// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// @OBSOLETE

function ret=TK_SetVar(Var,Val)
  warnobsolete("TCL_SetVar", "5.1");
  ret=TCL_SetVar(Var,Val)
endfunction
