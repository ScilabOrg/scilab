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

function Defaultlinkcolors_()
//
    Cmenu=[]
    [edited,options] = do_options(scs_m.props.options,'LinkColor') ;

    scs_m.props.options = options

    if edited then 
      // Acquire the current clicked window and put to "on" the pixmap mode
      gh_curwin = scf(%win) ;
      //** Clear the graphic window WITHOUT changing his pamaters ! :)
      drawlater() ;
        delete(gh_curwin.children.children) ; //** wipe out all the temp graphics object
        drawobjs(scs_m, gh_curwin) ;   //** re-create all the graphics object
      drawnow(); show_pixmap() ;      //** re-draw the graphic object and show on screen
     Cmenu = [] ; %pt = [];
    end

endfunction
