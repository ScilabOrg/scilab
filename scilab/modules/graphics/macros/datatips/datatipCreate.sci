// ====================================================================
// Copyright (C) INRIA -  Serge Steer Serge.Steer@inria.fr
//;
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// ====================================================================

function datatip_handle=datatipCreate(curve,index,ax)
//add a datatip at the given location of the given curve
//curve : handle on the curve
//index : index of the mesh point of the curve
//ax    : optional (the axes which contains the curve;
  bg=addcolor([255 255 238]/255);
  if type(curve)<>9|or(curve.type<>"Polyline") then
    error(msprintf(_("%s: Wrong size for input argument #%d: A ''%s'' handle expected.\n"),"datatipCreate",1,"Polyline"))
  end

  ud=datatipGetStruct(curve)
;
  if typeof(ud)<>'datatips' then
    //no datatips structure yet, initialize it
    if ~datatipsInitStruct(curve) then
      datatip_handle=[]
      return
    end
    ud=datatipGetStruct(curve)
  end
;
  formatfunction=ud.formatfunction;
  if argn(2)<3 then
    ax=curve.parent
    while ax.type<>"Axes" then ax=ax.parent,end
  else
    if type(ax)<>9|or(ax.type<>"Axes") then
      error(msprintf(_( "%s: Wrong size for input argument #%d: A ''%s'' handle expected.\n"),"datatipCreate",3,"Axes"))
    end
  end
  if size(index,'*')==1 then
    pt = curve.data(index,:);
  else
    pt=matrix(index,1,-1)
    index=[]
  end
  drawlater()
  //create the curve marker
  xpoly(pt(1),pt(2));point_handle=gce();
  if size(pt,'*')==3 then point_handle.data(3)=pt(3),end
  point_handle.mark_mode='on';
  point_handle.mark_style=11;;
  point_handle.mark_size_unit='point';
  point_handle.mark_size=6;
  point_handle.mark_background=bg;
  point_handle.mark_foreground=curve.foreground;
  point_handle.user_data=list(curve,index)
  //create the text box
  xstring(pt(1),pt(2),formatfunction(curve,pt,index))
;
  string_handle=gce();
  if size(pt,'*')==3 then string_handle.data(3)=pt(3),end
  string_handle.background=bg;
  string_handle.foreground=color('gray');
  string_handle.box='on';
  string_handle.fill_mode='on';
  string_handle.clip_state='off';
  string_handle.user_data=0; //automatic orientation
  setStringPosition(string_handle,pt)
  drawnow()
  datatip_handle= glue([string_handle,point_handle])

  ud.tips=[ud.tips datatip_handle]
  //store the datatips data structure into the curve data structure
  datatipSetStruct(curve,ud)
endfunction
