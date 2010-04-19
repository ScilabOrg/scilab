// ====================================================================
// Copyright (C) INRIA -  Serge Steer Serge.Steer@inria.fr
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// ====================================================================
function datatipsSetInterp(curve_handle,m)
//changes the tips interpolation modes for a given curve
// curve_handle : a handle on a polyline
// m            : %t (interpolation active) or %f
  if argn(2)==1 then m=%f,end
  ud=datatipsGetStruct(curve_handle)
  if typeof(ud)<>'datatips' then 
    datatipsInitStruct(curve_handle)
    ud=datatipsGetStruct(curve_handle)
  end
  ud.interpolate=m
  datatipsSetStruct(curve_handle,ud)
endfunction
