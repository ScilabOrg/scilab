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

function tt=sci2tcl(ttx)
  for t=['\',''"','[',']','{','}']
    ttx=strsubst(ttx,t,'\'+t)
  end
  //** strcat should be used : tt=strcat(ttx,"\n")
  if size(ttx,'*')<2 then tt=ttx,return,end
  tt=ttx(1)
  ttx=ttx(2:$)';
  for t=ttx
    tt=tt+'\n '+t
  end

endfunction
