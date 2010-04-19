// ====================================================================
// Copyright (C) INRIA -  Serge Steer Serge.Steer@inria.fr
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// ====================================================================

function setStringPosition(string_handle,pt)
//computes the position of the lower left corner of the string box
  r=xstringl(0,0,string_handle.text);r=r(3:4)';
  d=mark_usersize(point_handle);
  orient=string_handle.user_data;
  if orient==0 then //automatic
    t=[pt+r-ax.data_bounds(2,:)<0; //a l'interieur des max
       pt-r-ax.data_bounds(1,:)>0] //a l'interieur des min
    if and(t(1,:)) then //les deux sont inf aux max (upper right)
      orient=2;//1
    elseif and(t(2,:)) //les deux sont sup aux min (lower left)
      orient=3;//2
    elseif t(1,1)&t(2,2) //x est sup au min y est inf au max (upper left)
      orient=1;//3
    else//if t(1,2)*t(2,1) //y est sup au min   et x est inf au max (lower right)
      orient=4;
    end
  end
  select orient
  case 1 then //upper left
    string_handle.data=pt+[-d(1) d(2)]-[r(1) 0];
  case 2 then //upper right
     string_handle.data=pt+d;
  case 3 then //lower left
    string_handle.data=pt-d-r;
  case 4 then //lower right
     string_handle.data=pt+[d(1) -d(2)]-[0  r(2)];
  end
endfunction
function usize=mark_usersize(m)
  pix_size=10*(m.mark_size+1)
  [x1,y1]=xchange([0 pix_size],[0 pix_size],'i2f');
  usize=abs([x1(2)-x1(1) y1(2)-y1(1)])/20;
endfunction
