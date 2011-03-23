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

function syst=analyse(scs_m)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// Analyse le graphique pour rechercher les connections
//given	a block-diagram	representation of a linear system bloc2exp returns
//  its symbolic evaluation.  The	first element of the list blocd	must be	the
//  string 'blocd'.  Each	other element of this list (blocd(2),blocd(3),...) is
//  itself a list	of one the following types :


//  list('transfer','name_of_linear_system')


//  list('link','name_of_link',
//		 [number_of_upstream_box,upstream_box_port],
//		 [downstream_box_1,downstream_box_1_portnumber],
//		 [downstream_box_2,downstream_box_2_portnumber],
//		 ...)
//  The strings 'transfer' and 'links' are keywords which indicate the type of
//  element in the block diagram.
  nx=lstsize(scs_m.objs)
  nb=0
  syst=list()
  corresp=0*ones(nx,1)
  links=[]
  for k=1:nx
    o=scs_m.objs(k)

    if typeof(o)=='Block' then
      if o.model.sim=='super'|o.model.sim=='csuper' then	
	nb=nb+1
	syst(nb)=analyse(o.model)
	corresp(k)=nb
      else
	nb=nb+1
	select o.gui_name
	  case 'GAIN_f' then
	  syst(nb)=list('transfer',o.model)
	  corresp(k)=nb
	  case 'ECLAT_f' then
	  syst(nb)=list('transfer',ones(3,1))
	  corresp(k)=nb
	  case 'FILTRE_f' then
	  syst(nb)=list('transfer',model)
	  corresp(k)=nb
	  case 'SOM_f' then
	  syst(nb)=list('transfer',model)
	  corresp(k)=nb
	  case 'FORMEL_f' then
	  syst(nb)=list('transfer',model)
	  corresp(k)=nb
	  case 'IN_f' then
	  nb=nb-1
	  case 'OUT_f' then
	  nb=nb-1
	else
	  messagebox('Non linear systems are not implemented yet','modal')
	  syst(nb)=list('transfer',o.gui_name)
	  corresp(k)=nb
	end
      end
    else
      links=[links k]
    end
  end
  for k=links
    o=scs_m.objs(k)
    nb=nb+1
    o1=o.from(1)
    o2=o.to(1)
    o.from(1)=corresp(o.from(1))+1
    o.to(1)=corresp(o.to(1))+1
    if o1(4)=='IN_f' then
      syst(nb)=list('link','l'+string(nb),-1,o.to)
    elseif o2(4)=='OUT_f' then
      syst(nb)=list('link','l'+string(nb),o.from,-1)
    else
      syst(nb)=list('link','l'+string(nb),o.from,o.to)
    end
  end
  syst(0)='blocd'
endfunction
