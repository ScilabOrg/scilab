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
// optimbase_display --
//   Display the current settings in the console.
//
function this = optimbase_display ( this )
  warning ( msprintf ( gettext ( "%s: obsolete use of optimbase_display, use directly the variable instead."), "optimbase_display" ) )
  mprintf("Number of variables : %s\n", string(this.numberofvariables));
  str = string ( this )
  x0 = optimbase_cget (this,"-x0")
  srows = size(str,"r")
  mprintf("Initial Guess : [%s]\n" , _strvec(x0) );
  for i = 1 : srows
    mprintf("Initial Function Value :%s\n",_strvec(this.fx0));
    mprintf("%s\n",str(i))
    mprintf("Number of Inequality Constraints :%d\n",this.nbineqconst);
  end
endfunction

