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

function XcosMenuBrowser()
//

Cmenu = [] ; 

if super_path==[] then
   do_browser(scs_m);
else        
      Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
		       'Cmenu='"Browser'";%scicos_navig=[]';
		       '%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
                       'Cmenu='"Place in Browser'";%scicos_navig=[]'
		      ]  // not really needed to return anymore, may remove last line
//  messagebox("Browser can only be launched from main diagram.","modal") ;
end

endfunction
