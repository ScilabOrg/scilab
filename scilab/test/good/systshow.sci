//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function win=systshow(scs_m,win)
  [lhs,rhs]=argn(0)
  if rhs<2 then win=xget('window'),end
  xset('window',win);clf()
  wpar=scs_m.props

  wsiz=wpar.wpar
  options=wpar.options


  Xshift=wsiz(3)
  Yshift=wsiz(4)

  xset('wdim',wsiz(1),wsiz(2))
  [frect1,frect]=xgetech()
  xsetech([-1 -1 8 8]/6,[Xshift,Yshift ,Xshift+wsiz(1),Yshift+wsiz(2)])

  drawobjs(scs_m)

  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k)
    if typeof(o)=='Block' then
      model=o.model
      if model.sim=='super' then
	win=win+1
	win=systshow(model.rpar,win)
      end
    end
  end
endfunction
