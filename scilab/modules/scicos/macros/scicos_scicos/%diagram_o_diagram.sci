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

function bool = %diagram_o_diagram(d1, d2)
    // The first '%t' is there for backward compatibility. Indeed in Scilab 5 comparing two diagrams was comparing two mlists,
    // so even their headers could differ. If we are here, then the headers must be equal and we only need to check the fields contents.
    bool = [%t and(d1.props==d2.props) and(d1.objs==d2.objs) d1.version==d2.version and(d1.contrib==d2.contrib)];
endfunction
