// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function y = overloading_funeval(funname,x)
	[b,overfun] = overloading_exists(funname,x)
	if ( ~b ) then
		overloading_error(funname,x)
	end
	instr = "y =" + overfun + "(x)"
	ierr = execstr(instr,"errcatch")
	if ( ierr <> 0 ) then
		lamsg = lasterror()
		lclmsg = gettext("%s: Unable to evaluate overloading function %s: %s")
		error(msprintf(lclmsg,"overloading_funeval",funpattern,lamsg))
	end
endfunction
