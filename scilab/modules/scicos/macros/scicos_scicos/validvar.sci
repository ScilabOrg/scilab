//  Scicos
//
//  Copyright (C) INRIA - Serge Steer <serge.steer@inria.fr>
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

function r=validvar(s)
    //Serge Steer, 08/10/2007
    //Alan restricted validvar for operation, 29/03/08

    //check if the string s is a valid identifier
    s=stripblanks(s)
    r=%f
    if size(s,"*")<>1 then return, end
    if s=="" then return, end

    //create a function with s as single statement
    if execstr("deff(''foo'',s)","errcatch") then return, end
    t=macr2tree(foo) //syntax tree
    instr=t.statements(2) //ignore leading and trailing EOL

    //lhs should be "ans"
    if size(instr.lhs)<>1 then return,end
    if instr.lhs(1).name<>"ans" then return,end

    //expression should be just a reference to s
    if typeof(instr.expression)<>"operation" then return, end
    if instr.expression.operator<>"ext" then return,end
    if size(instr.expression.operands)<>1 then return,end
    if instr.expression.operands(1).name<>s  then return,end

    r=%t
endfunction
