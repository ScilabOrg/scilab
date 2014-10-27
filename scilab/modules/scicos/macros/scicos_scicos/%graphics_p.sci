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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function %graphics_p(graphics)
    //fn=getfield(1,graphics)
    try
        // 'pin' property exists: Block
        graphics.pin;
        fn=["graphics" "orig" "sz" "flip" "theta" "exprs" "pin" "pout" "pein" "peout",..
        "gr_i" "id" "in_implicit" "out_implicit" "in_style" "out_style" "in_label" "out_label" "style"];
    catch
        // Annotation
        fn=["graphics" "orig" "sz" "exprs"];
    end

    //if type(graphics) == 17 then // Annotation
    //    fn=["graphics" "orig" "sz" "exprs"];
    //else // Type 128: Block
    //    fn=["graphics" "orig" "sz" "flip" "theta" "exprs" "pin" "pout" "pein" "peout",..
    //    "gr_i" "id" "in_implicit" "out_implicit" "in_style" "out_style" "in_label" "out_label" "style"];
    //end

    for k=2:size(fn,"*")
        mprintf("%s\n", sci2exp(eval("graphics."+fn(k)),fn(k)))
    end
endfunction
