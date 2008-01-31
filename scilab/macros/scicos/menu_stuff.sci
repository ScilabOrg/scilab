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

function menu_stuff()

if ~MSDOS then
  delmenu(curwin,'3D Rot.')
  delmenu(curwin,'UnZoom')
  delmenu(curwin,'Zoom')
  delmenu(curwin,'Edit')
  delmenu(curwin,'File')
  delmenu(curwin,'Insert')
else
  hidetoolbar(curwin)
 // French
  delmenu(curwin,'&Fichier')
  delmenu(curwin,'&Editer')
  delmenu(curwin,'&Outils')
  delmenu(curwin,'&Inserer')
  // English
  delmenu(curwin,'&File')
  delmenu(curwin,'&Edit')
  delmenu(curwin,'&Tools')
  delmenu(curwin,'&Insert')
  end
menuss=menus;
menuss(1)=menus(1)(2:$);
menubar(curwin,menuss)
	  
endfunction
