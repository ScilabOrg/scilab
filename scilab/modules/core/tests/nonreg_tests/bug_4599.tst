// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 4599 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4599
//
// <-- Short Description -->
// It is possible create many different variables with the same name in the same scope!!
clear M

function ldcsv(),
  if exists('M')==0 then M=1,;else M=M+1;end
  execstr('[M]=return(M)'),
endfunction


//----------------------------------------
execstr ('ldcsv()')
execstr ('ldcsv()')
if size(find(who('local')=='M'),'*')<>1 then pause,end
if M<>2 then pause,end
//----------------------------------------

function [n,M]=foo
  ldcsv()
  ldcsv()
  n=size(find(who('local')=='M'),'*')
endfunction
clear M;[n,M]=foo();
if n<>1 then pause,end
if M<>2 then pause,end
//----------------------------------------
function [n,M]=foo1
  execstr ('ldcsv()')
  execstr ('ldcsv()')
  n=size(find(who('local')=='M'),'*')
endfunction
clear M;[n,M]=foo1();
if n<>1 then pause,end
if M<>2 then pause,end

//----------------------------------------

function [n,M]=foo1
  ldcsv()
  ldcsv()
  n=size(find(who('local')=='M'),'*')
endfunction
clear M;[n,M]=foo1();
if n<>1 then pause,end
if M<>2 then pause,end

[foo1(),size(find(who('local')=='M'),'*')]
//----------------------------------------

function [n,M1]=foo2(),
  [n1,M1]=foo1(),
  n2=size(find(who('local')=='M'),'*'),
  n=[n1,n2],
endfunction
function [n,M]=foo1
  ldcsv()
  ldcsv()
  n=size(find(who('local')=='M'),'*')
endfunction
clear M;[n,M]=foo2();
if n<>1 then pause,end
if M<>2 then pause,end

//----------------------------------------
