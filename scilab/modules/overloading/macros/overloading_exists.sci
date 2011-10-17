// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function [b,overfun] = overloading_exists(funname,x)
	funpattern = "%s_"+funname
	instr = "overfun = msprintf(""%%""+funpattern,typeof(x))"
	ierr = execstr(instr,"errcatch")
	if ( ierr <> 0 ) then
		lamsg = lasterror()
		lclmsg = gettext("%s: Unable to format the function pattern %s: %s")
		error(msprintf(lclmsg,"overloading_exists",funpattern,lamsg))
	end
	b = (exists(overfun)==1)
endfunction
