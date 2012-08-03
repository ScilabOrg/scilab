// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7163 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bugAdmin_II/show_bug.cgi?id=7163
//
// <-- Short Description -->
//   assignment index : are translated into eye()
function foo
  a(:)
  a(:,1)
  a(1,:)
  a(:,:)

  a(:)=3
  a(:,1)=4
  a(1,:)=5
  a(:,:)=6
endfunction
t=stripblanks(fun2string(foo));
t=strsubst(t(2:$-1),' ','');
tref=["a(:)";"a(:,1)";"a(1,:)";"a(:,:)";"";"a(:)=3";"a(:,1)=4";"a(1,:)=5";"a(:,:)=6"];
if or(t<>tref) then pause,end
