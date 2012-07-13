// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 1469 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1469
//
// <-- Short Description -->
//    Comments modify line numbering in functions
//
//    What you don't believe?
//
//    Try this:
//
//    1st test, no comment in line 1:
//
//    -->function foo1()
//    -->  function foo3
//    -->  endfunction
//    -->  disp("line a");
//    -->  disp("line b");
//    -->  disp("line c");
//    -->endfunction
//
//    -->setbpt("foo1",[3,4])
//
//    -->foo1()
//    Stop after row     3 in function foo1 :
// ...

//test of line numbering in  functions, handling initial comments


// ================ Test 1 =====================================================

function foo,endfunction
L=macr2lst(foo);
R1=L(4)(1)=='15'&size(L)==6
if ~R1 then pause,end
clear foo;

// ================ Test 2 =====================================================

function foo,,endfunction
L=macr2lst(foo);
R2=L(4)(1)=='15'&size(L)==6
if ~R2 then pause,end
clear foo;

// ================ Test 3 =====================================================

function foo(),endfunction
L=macr2lst(foo);
R3=L(4)(1)=='15'&size(L)==6
if ~R3 then pause,end
clear foo;

// ================ Test 4 =====================================================

function foo(),a=1,endfunction
L=macr2lst(foo);
R4=L(4)(1)=='6'&size(L)==8
if ~R4 then pause,end
clear foo;

// ================ Test 5 =====================================================

function foo
endfunction
L=macr2lst(foo);
R5=L(4)(1)=='15'&size(L)==7
if ~R5 then pause,end
clear foo;

// ================ Test 6 =====================================================

function foo,
endfunction
L=macr2lst(foo);
R6=L(4)(1)=='15'&size(L)==7
if ~R6 then pause,end
clear foo;

// ================ Test 7 =====================================================

function foo//ZZZZ
endfunction
L=macr2lst(foo);
R7=L(4)(1)=='31'&L(5)(1)=='15'&size(L)==8
if ~R7 then pause,end
clear foo;

// ================ Test 8 =====================================================

function foo,//ZZZZ
endfunction
L=macr2lst(foo);
R8=L(4)(1)=='31'&L(5)(1)=='15'&size(L)==8
if ~R8 then pause,end
clear foo;

// ================ Test 9 =====================================================

function foo
//ZZZZ
endfunction
L=macr2lst(foo);
R9=L(4)(1)=='15'&L(5)(1)=='31'&size(L)==9
if ~R9 then pause,end
clear foo;

// ================ Test 10 ====================================================

function foo,//ZZZZ
  a=1
endfunction
L=macr2lst(foo);
R10=L(4)(1)=='31'&L(5)(1)=='15'&L(6)(1)=='6'&size(L)==11
if ~R10 then pause,end
clear foo;

// ================ Test 11 ====================================================

function loc=foo,[l,w]=where(),loc=l(1),endfunction
R11= and(foo()==1)
if ~R11 then pause,end
clear foo;

// ================ Test 12 ====================================================

function loc=foo,//ZZZZ
  [l,w]=where(),loc=l(1)
endfunction
R12= and(foo()==2)
if ~R12 then pause,end
clear foo;

// ================ Test 13 ====================================================

function loc=foo
//ZZZZ
  [l,w]=where(),loc=l(1)
endfunction
R13= and(foo()==3)
if ~R13 then pause,end
clear foo;

// ================ Test 14 ====================================================

function loc=foo1()
  [l,w]=where(),loc=l(1)
  function loc=foo3(loc)
    [l,w]=where(),loc=[loc,l(1:2)']
  endfunction
  [l,w]=where(),loc=[loc,l(1)]
  loc=foo3(loc)
endfunction
R14= and(foo1()==[2,6,2,7])
if ~R14 then pause,end
clear foo1;

// ================ Test 15 ====================================================

function loc=foo1()//ZZZZZZZ
  [l,w]=where(),loc=l(1)
  function loc=foo3(loc)
    [l,w]=where(),loc=[loc,l(1:2)']
  endfunction
  [l,w]=where(),loc=[loc,l(1)]
  loc=foo3(loc)
endfunction
L=macr2lst(foo1);
R15=and(foo1()==[2,6,2,7])&(L(4)(1)=='31')&(L(5)=='15')
if ~R15 then pause,end
clear foo1;

// ================ Test 16 ====================================================

function loc=foo1()
  //ZZZZZZZ
  [l,w]=where(),loc=l(1)
  function loc=foo3(loc)
    [l,w]=where(),loc=[loc,l(1:2)']
  endfunction
  [l,w]=where(),loc=[loc,l(1)]
  loc=foo3(loc)
endfunction
L=macr2lst(foo1);
R16=and(foo1()==[3,7,2,8])&(L(4)=='15')&(L(5)(1)=='31')
if ~R16 then pause,end
clear foo1;

// ================ Test 17 ====================================================

//pb execution des macros non compilee
deff('y=foo(a,b,c)','y=a+b+c','n')
R17=foo('aaa','bbb','cc')=='aaabbbcc'
if ~R17 then pause,end
clear foo

// ================ Test 18 ====================================================

deff('y=foo(a,b,c)',['','y=a+b+c'],'n')
R18=foo('aaa','bbb','cc')=='aaabbbcc'
if ~R18 then pause,end

// ================ Test 19 ====================================================

comp(foo)
R19=foo('aaa','bbb','cc')=='aaabbbcc'
if ~R19 then pause,end
clear foo

// ================ Test 20 ====================================================

//function definition nested in a or loop!
z=0;R20=%t
for k=1:2
 function y=foo()
   y=k
 endfunction
 L=macr2lst(foo);
 R20=R20&(L(4)=='15')&(L(5)(1)=='2');
 z=z+foo();
end
R20=R20&z==3
if ~R20 then pause,end

// ================ Test 21 ====================================================

//test avec getf
t=['function foo,//ZZZZ'
'  a=1'
'endfunction'];
mputl(t,TMPDIR+'/foo.sci');
warning('off');
exec(TMPDIR+'/foo.sci')
warning('on');
L=macr2lst(foo); //getf ignores declaration line comments
R21=L(4)(1)=='31'&L(5)(1)=='15'&size(L)==11
if ~R21 then pause,end
clear foo;

// ================ Test 22 ====================================================

t=['function foo'
'//ZZZZ'
'  a=1'
'endfunction'];
mputl(t,TMPDIR+'/foo.sci');
exec(TMPDIR+'/foo.sci')
L=macr2lst(foo); //getf ignores declaration line comments
R22=L(4)(1)=='15'&L(5)(1)=='31'&L(6)(1)=='15'&size(L)==12
if ~R22 then pause,end
clear foo;

// ================ Test 23 ====================================================
function foo()
  function bar()
    a=1
endfunction
endfunction
t=fun2string(foo,'foo')
if size(t,1)<>5 then pause,end

clear foo
function foo()
  function bar()
    a=1
 endfunction
endfunction
t1=fun2string(foo,'foo')
if or(t<>t1) then pause,end
// ================ Test 24 ====================================================
function test;endfunction
L=macr2lst(test)
if size(L)<>6 then pause,end
if L(4)<>'15'|L(5)<>'99'| L(6)<>'15' then pause,end
