// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Vincent Liard
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// RPEM unit tests

// substraction on lists (for lists whose elements allow the minus operator)
function r=list_substract(l1,l2)
r = list();
for i=1:length(l1)
  r(i) = l1(i) - l2(i);
end
endfunction

// overload minus operator for lists
//deff('x = %l_s_l(l1,l2)', 'x = list_substract(l1,l2)');
function  x = %l_s_l(l1,l2)
    x = list_substract(l1,l2)
endfunction


// absolute value on lists
//function r=list_abs(l)
//r=list()
//for i=1:length(l)
//  r(i) = abs(l(i))
//end
//endfunction

// overload absolute value on lists
//deff('y = %l_abs(x)', 'y = list_abs(x)');

//function  y = %l_abs(x)
function y=list_abs(x)
y=list()
for i=1:length(x)
    y = abs(x(i))
end
endfunction

// overall maximum on structured objects
// list(list(1,2,3),list(2,list(7,11),4)) -> 11
// [4,[[7,[11,3,[1,[20],[]],2]],2]] -> 20
function m=list_max(l)
m=-%inf
if typeof(l) == 'constant' then
  m = max(l)
else
  for e=l
    m = max(list_max(e), m)
  end
end
endfunction

// unfortunately, max can't be overloaded on lists
// (since list max is already hardwrired)

// check whether objects a and b differ from less than 2*epsilon
// (error on a and b: eps => error on a-b: 2eps)
// (works on any list-matrix combination)
// epsilon defaults to %eps if not provided by calling sequence
function r=match(a,b,epsilon)
if ~isdef('epsilon') then
  epsilon = %eps;
end
r = (list_max(list_abs(a-b)) <= 2*epsilon);
endfunction

// because of test mechanism restrictions...
if ~isdef('bugmes') then
  function bugmes()
  pause
  endfunction
end

// compare objects a and b, pause if differ from more than epsilon
// epsilon defaults to %eps if not provided by calling sequence
// would better rely on match's default epsilon but I lack inspiration in this thursday's early afternoon
function check(a,b,epsilon)
if ~isdef('epsilon') then
  epsilon = %eps;
end
if ~match(a,b,epsilon) then
  bugmes();
end
endfunction
