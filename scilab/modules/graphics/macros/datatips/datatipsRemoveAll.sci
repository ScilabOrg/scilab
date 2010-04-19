// ====================================================================
// Copyright (C) INRIA -  Serge Steer Serge.Steer@inria.fr
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// ====================================================================

function datatipRemoveAll(curve_handles)
//remove all the datatips for the given curves
  if type(curve_handles)<>9 then
    error(msprintf(_( "%s: Wrong type for input argument: ''%s'' handle expected.\n"),"datatipRemoveAll","1"))
  end

  drawlater()

  for k=1:size(curve_handles,'*')
    ck=curve_handles(k);
    ud=datatipGetStruct(ck)// the curve datatips data structure
    if typeof(ud)=='datatips' then
      for i=1:size(ud.tips,'*'), delete(ud.tips(i));end
      ud.tips=[]
      datatipSetStruct(ck,ud)
    end
  end
  drawnow()
endfunction
