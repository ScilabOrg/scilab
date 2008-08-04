
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_do_replot(GraphList)
  ge_disablemenus()
  
  ge_set_winsize()
  ge_axes_handle=gca()
  if ~isempty(ge_axes_handle.children) then
    delete(ge_axes_handle.children);
  end
  set(ge_axes_handle,'user_data',tlist(['gindex','node','edge'],[],[]))
  ge_draw_graph(GraphList)
  ge_enablemenus();

endfunction
