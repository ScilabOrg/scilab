// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
// <-- Non-regression test for bug 8995 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8995
//
// <-- Short Description -->
// Problem with insertion in empty matrix

function linenumber()
  [linenum,mac]=where()
  if argn(2)<1 then
    mprintf("Problem detected at line #%d of %s\n",linenum(2),mac(2))
  end
endfunction
function test_8995()
  S=list(1,int32(1),'s',%t,%s,sparse(1),sparse(%t));
  for scalar=S
    if execstr("A=[];A($,:)=scalar","errcatch")==0 then linenumber();pause,end
    if execstr("A=[];A(:,$)=scalar","errcatch")==0 then linenumber();pause,end
    if execstr("A=[];A($)=scalar","errcatch")==0 then linenumber();pause,end
    if execstr("A=[];A($,1)=scalar","errcatch")==0 then linenumber();pause,end
    if execstr("A=[];A(1,$)=scalar","errcatch")==0 then linenumber();pause,end

    A=[];A($+1,1:$)=scalar;
    if A<>[] then linenumber();pause,end
    A=[];A(1:$,$+1)=scalar;
    if A<>[] then linenumber();pause,end
    A=[];A(:,:)=scalar;
    if A<>[] then linenumber();pause,end
    A=[];A(1:$,:)=scalar;
    if A<>[] then linenumber();pause,end
    A=[];A(:, 1:$)=scalar;
    if A<>[] then linenumber();pause,end
    A=[];A(1:$,1:$)=scalar;
    if A<>[] then linenumber();pause,end
    A=[];A(:,%f)=scalar;
    if A<>[] then linenumber();pause,end
    A=[];A(%f,:)=scalar;
    if A<>[] then linenumber();pause,end

    A=[];A($+1,:)=scalar;
    if A<>scalar then linenumber();pause,end
    A=[];A(:,$+1)=scalar;
    if A<>scalar then linenumber();pause,end

    A=[];A(%t,:)=scalar;
    if A<>scalar then linenumber();pause,end
    A=[];A(:,%t)=scalar;
    if A<>scalar then linenumber();pause,end

  end

  R=list(1:2,int32(1:2),['s' 't'],[%t %f],[%s %s+1],1:2,        [%t %f],  sparse(1:2),sparse([%t %f]))
  S=list(1,  int32(1),   's',      %t,     %s,      sparse(1),  sparse(%t), 1,         %t);
  for row=R
    if execstr("A=[];A([$+1 $+2],1:$)=row'';","errcatch")==0 then linenumber();pause,end
    if execstr("A=[];A(1:$,[$+1 $+2])=row;","errcatch")==0 then linenumber();pause,end
    A=[];A([$+1 $+2],:)=row';
    if A<>row' then linenumber();pause,end
    A=[];A(:,[$+1 $+2])=row;
    if A<>row then linenumber();pause,end
    A=[];A([%t %t],:)=row';
    if A<>row' then linenumber();pause,end
    A=[];A(:,[%t %t])=row;
    if A<>row then linenumber();pause,end
  end
  
  for  k=1:size(R)
    row=R(k); scalar=S(k);
    A=row;
    A(:,:)=row;
    if A<>row then linenumber();linenumber();pause,end
    A=row;
    A(:,:)=scalar;
    if or(type(scalar)==[5 6]) then scalar=full(scalar),end
    if or(size(A)<>size(row))|or(A<>scalar) then linenumber();pause,end
  end
  
  
  
endfunction
test_8995()
