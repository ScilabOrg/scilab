//  Scicos
//
//  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
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

function bool = %Link_o_Link(l1, l2)
    // The first '%t' is there for backward compatibility. Indeed in Scilab 5 comparing two links was comparing two mlists,
    // so even their headers could differ. If we are here, then the headers must be equal and we only need to check the fields contents.
    bool = [%t and(l1.xx==l2.xx) and(l1.yy==l2.yy) l1.id==l2.id and(l1.thick==l2.thick) and(l1.ct==l2.ct) and(l1.from==l2.from) and(l1.to==l2.to)];
endfunction
