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

function XcosMenuUpToMain()
  Cmenu=[]
  if super_block then
    //each line of Scicos_commands are directive to be executed by scicos
    //one after the other.
    //%diagram_path_objective=[] stands for up to main
      Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
		       'Cmenu=[];%scicos_navig=[]';
		       'xselect()'
		      ]
  else
      messagebox('This is already the main diagram;','modal')
  end
endfunction
