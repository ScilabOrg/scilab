// ====================================================================
// Copyright (C) INRIA -  Serge Steer Serge.Steer@inria.fr
//;
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// ====================================================================

function datatipManagerMode(varargin)
//Toggle or sets the interactive mode for datatips edition.;
//  This function is called by the datatips menu callbacks.;
  old=[] //to preserve cuurent figure
  if winsid()<>[] then  old=gcf();end
  select size(varargin)
  case 0 then //toggle mode
    fig=gcf()
    action='toggle'
  case 1 then
    if type(varargin(1))==10 then
      action=varargin(1)
      fig=gcf()
    elseif type(varargin(1))==9 then
      fig=varargin(1)
      action='toggle'
    elseif type(varargin(1))==1 then
      fig=scf(varargin(1))
      if old<>[] then scf(old),end
      action='toggle'
    end
  case 2 then
    if type(varargin(1))==10 then
      action=varargin(1)
      fig=varargin(2)
    elseif type(varargin(1))==9 then
      fig=varargin(1)
      action=varargin(2)
    elseif type(varargin(1))==1 then
      fig=scf(varargin(1))
      if old<>[] then scf(old),end
      action=varargin(2)
    end
  else
    error(msprintf(_("%s: too many input arguments"),'datatipManagerMode'))
  end
  if fig.event_handler == '' then
    fig.event_handler = "datatipEventhandler"
  end
  select action
  case 'on'
    fig.event_handler_enable = "on"
  case 'off'
    fig.event_handler_enable = "off"
  case 'toggle'
    if  fig.event_handler_enable == "on" then
      fig.event_handler_enable = "off"
    else
      fig.event_handler_enable = "on"
    end
  end
endfunction

function datatipEventhandler(win,x,y,ibut)
//The event handler which rules the datatips interactive edition;
  if ibut<0 then return,end
  old=gcf()
  fig=scf(win);
  fig.event_handler_enable = "off";
  ax=getAxes([x,y])
  sca(ax)
  //change the pixel coordinates to user coordinates
  [x,y]=xchange(x,y,'i2f');pt=[x,y]
  if or(ibut==[0 3]) then //select a point on a curve

    //find  curves in the current axes
    curve_handles=datatipGetEntities(ax);
    if curve_handles==[] then;
      scf(old);
      return,
    end

    //check for a text box present near the selected point
    [k,l]=datatipLookfor(curve_handles,pt)
    if k<>[] then;
      ud=datatipGetStruct(curve_handles(k))
      tip_handle=ud.tips(l)
      datatipMove(tip_handle),
      fig.event_handler_enable = "on";
      return,
    end
    dmin=%inf,l=0
    for k=1:size(curve_handles,'*')
      ck=curve_handles(k)
      //find the polylines point with realizes the minimal euclidean distance with pt
      [d,ptp,ind]=orthProj(ck.data,pt)
      if ind<>[] then
        d= pixDist(ptp,pt);
        if d<dmin then
          dmin=d
          l=ind
          kmin=k,
          ptmin=ptp
        end
      end
    end
    if dmin<10 then;
      //small minimal distance, create a text box at this point (or move
      //an existing tip at this point
      ud=datatipGetStruct(curve_handles(kmin))// the curve datatips data structure
      if typeof(ud)<>'datatips' then;
        datatipInitStruct(curve_handles(kmin))
        ud=datatipGetStruct(curve_handles(kmin))
      end
      if ud.interpolate then
        datatipCreate(curve_handles(kmin),ptmin);
      else
        datatipCreate(curve_handles(kmin),l); //
      end
    else
      datatipRemoveAll(curve_handles)
    end
  elseif or(ibut==[2 5]) then
    curve_handles=datatipGetEntities(ax);
    [k,l]=datatipLookfor(curve_handles,pt)
    if k<>[] then;
      select x_choose([_("Delete all datatips for the selected curve")
                       _("Delete selected datatip")
                       _("Change the curve tip display function")
                       _("Enable interpolation")
                       _("Disable interpolation")],'')
      case 0 then //do nothing
      case 1 then;
        datatipRemoveAll(curve_handles(k))
      case 2 then
        datatipRemove(curve_handles(k),l)
      case 3 then
        datatipSetDisplay(curve_handles(k))
      case 4 then
        datatipSetInterp(curve_handles(k),%T)
      case 5 then
        datatipSetInterp(curve_handles(k),%F)
      end
    end
  elseif or(ibut==[1 4]) then
    curve_handles=datatipGetEntities(ax);
    [k,l]=datatipLookfor(curve_handles,pt)
    if k<>[] then;
      ud=datatipGetStruct(curve_handles(k))// the curve datatips data structure
      tip_handle=ud.tips(l);
      orient=["automatic" "upper left" "upper right", "lower left", "lower right"]
      orientations=[_("automatic") _("upper left") _("upper right"), _("lower left"), _("lower right")]
      r=x_choose(orientations,_("Select tip orientation"))
      if r<>0 then
        datatipSetOrientation(tip_handle,orient(r))
      end
    end
  end
  fig.event_handler_enable = "on";
  scf(old)
endfunction

function [curve_index,tip_index]=datatipLookfor(curve_handles,pt)
//looks for a datatip in the neighborhood of a given point
//curve_handles:  a vector of curves which are supposed to have datatips
//pt           :  a 2D point in pixels
//curve_index  :  the index of corresponding curve in curve_handles
//tip_index    :  the datatip index for the curve;
  for curve_index=1:size(curve_handles,'*')
    ud=datatipGetStruct(curve_handles(curve_index))
    if typeof(ud)=='datatips' then
      tips=ud.tips
      for tip_index=1:size(tips,'*')
        d=pixDist(tips(tip_index).children(1).data,pt)
        if d<10 then return, end
      end
    end
  end
  curve_index=[];tip_index=[];
endfunction


function ax_handle=getAxes(pt,fig)
//Returns the axes handle where a point given in pixel lies in the
//current (or given) figure
// pt        : the vector or coordinates in pixel in the figure
// fig       : optionnal handle on the figure
// ax_handle : selected axes handle
  if argn(2)==1 then fig=gcf(),end
  axes=get(fig,'children');
  axes(axes.type=='uimenu')=[];
  if size(axes,'*')==1 then ax_handle=axes,return,end
  sz=fig.axes_size
  for k=1:size(axes,'*')
    ax_handle=axes(k)
    xbounds=ax_handle.axes_bounds(:,1)*sz(1)
    ybounds=ax_handle.axes_bounds(:,2)*sz(2)
    if prod(xbounds-pt(1))<0&prod(ybounds-pt(2))<0 then break,end
  end
endfunction
