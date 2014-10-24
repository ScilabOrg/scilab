//  Scicos
//
//  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
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

function %Link_p(l)

    mprintf("Link    :\n");

    linkFields = ["xx", "yy", "id", "thick", "ct", "from", "to"];

    for i = 1:size(linkFields, "*")
        res = string(eval("l."+linkFields(i)));
        sizeRes = size(res, "*")
        if sizeRes == 0 then
            mprintf("          %s: %s\n", linkFields(i), "[]")
        else
            mprintf("          %s: %s\n", linkFields(i), strcat(res, " "))
        end
    end

endfunction
