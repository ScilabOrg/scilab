// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=xload(fil,num)

	if argn(2)<1 | argn(2)>2 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "xload", 1, 2));
	end

  if  argn(2)==2 then scf(num),end
  f=gcf()
  res = execstr('load(fil)','errcatch') ;
  if res==0 then
    f.visible='on'
    f.immediate_drawing='on';
  else
    error('Given file is not a graphic one') ;
  end

endfunction
