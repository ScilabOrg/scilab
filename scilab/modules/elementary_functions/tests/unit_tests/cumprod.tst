// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI TEST -->

//cumprod
//Empty matrices
T=list(list(),list('native'),list('double'));
for typ=T
  if cumprod([],typ(:))<>[] then pause,end
  if cumprod([],'*',typ(:))<>[] then pause,end
  if cumprod([],1,typ(:))<>[] then pause,end
  if cumprod([],2,typ(:))<>[] then pause,end
  if cumprod([],3,typ(:))<>[] then pause,end
end

//=======================================================================
//float matrices
d=[1 10;254 9];
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumprod(d,typ(:))<>[1,2540;254,22860]) then pause,end
  if or(cumprod(d,'*',typ(:))<>[1,2540;254,22860]) then pause,end
  if or(cumprod(d,1,typ(:))<>[1,10;254,90]) then pause,end
  if or(cumprod(d,2,typ(:))<>[1,10;254,2286]) then pause,end
  if or(cumprod(d,3,typ(:))<>d) then pause,end
end

//hypermatrices of floats
d=[1 10;254 9];d(1,1,2)=1;
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumprod(d,typ(:))<>hypermat([2,2,2],[1;254;2540;22860;22860;0;0;0])) then pause,end
  if or(cumprod(d,'*',typ(:))<>hypermat([2,2,2],[1;254;2540;22860;22860;0;0;0])) then pause,end
  if or(cumprod(d,1,typ(:))<>hypermat([2,2,2],[1;254;10;90;1;0;0;0])) then pause,end
  if or(cumprod(d,2,typ(:))<>hypermat([2,2,2],[1;254;10;2286;1;0;0;0])) then pause,end
  if or(cumprod(d,3,typ(:))<>hypermat([2,2,2],[1;254;10;9;1;0;0;0])) then pause,end
  if or(cumprod(d,5,typ(:))<>d) then pause,end
end

//=======================================================================
//Integers
i=uint8([1 10;254 9]);
for typ=list(list(),list('native'));
  if or(cumprod(i,typ(:))<>uint8([1,236;254,76])) then pause,end
  if or(cumprod(i,'*',typ(:))<>uint8([1,236;254,76])) then pause,end
  if or(cumprod(i,1,typ(:))<>uint8([1,10;254,90])) then pause,end
  if or(cumprod(i,2,typ(:))<>uint8([1,10;254,238])) then pause,end
  if or(cumprod(i,3,typ(:))<>i) then pause,end
end

if or(cumprod(i,'double')<>[1,2540;254,22860]) then pause,end
if or(cumprod(i,'*','double')<>[1,2540;254,22860]) then pause,end
if or(cumprod(i,1,'double')<>[1,10;254,90]) then pause,end
if or(cumprod(i,2,'double')<>[1,10;254,2286]) then pause,end
if or(cumprod(i,3,'double')<>double(i)) then pause,end

//with hypermatrices
i=uint8([1 10;254 9]);i(1,1,2)=uint8(1);
for typ=list(list(),list('native'));
  if or(cumprod(i,typ(:))<>hypermat([2,2,2],uint8([1;254;236;76;76;0;0;0]))) then pause,end
  if or(cumprod(i,'*',typ(:))<>hypermat([2,2,2],uint8([1;254;236;76;76;0;0;0]))) then pause,end
  if or(cumprod(i,1,typ(:))<>hypermat([2,2,2],uint8([1;254;10;90;1;0;0;0]))) then pause,end
  if or(cumprod(i,2,typ(:))<>hypermat([2,2,2],uint8([1;254;10;238;1;0;0;0]))) then pause,end
  if or(cumprod(i,3,typ(:))<>hypermat([2,2,2],uint8([1;254;10;9;1;0;0;0]))) then pause,end
  if or(cumprod(i,5,typ(:))<>i) then pause,end
end


if or(cumprod(i,'double')<>hypermat([2,2,2],[1;254;2540;22860;22860;0;0;0])) then pause,end
if or(cumprod(i,'*','double')<>hypermat([2,2,2],[1;254;2540;22860;22860;0;0;0])) then pause,end
if or(cumprod(i,1,'double')<>hypermat([2,2,2],[1;254;10;90;1;0;0;0])) then pause,end
if or(cumprod(i,2,'double')<>hypermat([2,2,2],[1;254;10;2286;1;0;0;0])) then pause,end
if or(cumprod(i,3,'double')<>hypermat([2,2,2],[1;254;10;9;1;0;0;0])) then pause,end
if or(cumprod(i,5,'double')<>double(i)) then pause,end

//=======================================================================
//Matrices of Polynomials 
s=%s;p=[s s+1;s^2 0];
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumprod(p,typ(:))<>[s,s^3+s^4;s^3,0*s]) then pause,end
  if or(cumprod(p,'*',typ(:))<>[s,s^3+s^4;s^3,0*s]) then pause,end
  if or(cumprod(p,1,typ(:))<>[s,1+s;s^3,0*s]) then pause,end
  if or(cumprod(p,2,typ(:))<>[s,s+s^2;s^2,0*s]) then pause,end
  if or(cumprod(p,3,typ(:))<>p) then pause,end
end

//with hypermatrices
s=%s;p=[s s+1;s^2 0];p(1,1,2)=-1;
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumprod(p,typ(:))<>hypermat([2,2,2],[s;s^3;s^3+s^4;0*s;0*s;0*s;0*s;0*s])) then pause,end
  if or(cumprod(p,'*',typ(:))<>hypermat([2,2,2],[s;s^3;s^3+s^4;0*s;0*s;0*s;0*s;0*s])) then pause,end
  if or(cumprod(p,1,typ(:))<>hypermat([2,2,2],[s;s^3;1+s;0*s;-1;0*s;0*s;0*s])) then pause,end
  if or(cumprod(p,2,typ(:))<>hypermat([2,2,2],[s;s^2;s+s^2;0*s;-1;0*s;0*s;0*s])) then pause,end
  if or(cumprod(p,3,typ(:))<>hypermat([2,2,2],[s;s^2;1+s;0*s;-s;0*s;0*s;0*s])) then pause,end
  if or(cumprod(p,5,typ(:))<>p) then pause,end
end
//=======================================================================
//Matrices of rationals
s=%s;r=1.0 ./[s s+1;s^2 1];
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumprod(r,typ(:))<>[1,1;1,1]./[s,s^3+s^4;s^3,s^3+s^4]) then pause,end
  if or(cumprod(r,'*',typ(:))<>[1,1;1,1]./[s,s^3+s^4;s^3,s^3+s^4]) then pause,end
  if or(cumprod(r,1,typ(:))<>[1,1;1,1]./[s,1+s;s^3,1+s]) then pause,end
  if or(cumprod(r,2,typ(:))<>[1,1;1,1]./[s,s+s^2;s^2,s^2]) then pause,end
  if or(cumprod(r,3,typ(:))<>r) then pause,end
end

//=======================================================================
///Matrices of boolean

b=[%t %t;%f %t];
for typ=list(list(),list('double'));
  if or(cumprod(b,typ(:))<>[1,0;0,0]) then pause,end
  if or(cumprod(b,'*',typ(:))<>[1,0;0,0]) then pause,end
  if or(cumprod(b,1,typ(:))<>[1,1;0,1]) then pause,end
  if or(cumprod(b,2,typ(:))<>[1,1;0,0]) then pause,end
  if or(cumprod(b,3,typ(:))<>double(b)) then pause,end
end
if or(cumprod(b,'native')<>[%t,%f;%f,%f]) then pause,end
if or(cumprod(b,'*','native')<>[%t,%f;%f,%f]) then pause,end
if or(cumprod(b,1,'native')<>[%t,%t;%f,%t]) then pause,end
if or(cumprod(b,2,'native')<>[%t,%t;%f,%f]) then pause,end
if or(cumprod(b,3,'native')<>b) then pause,end

//with hypermatrices
b=[%t %t;%f %t];b(1,1,2)=%f;
for typ=list(list(),list('double'));
  if or(cumprod(b,typ(:))<>hypermat([2,2,2],[1;0;0;0;0;0;0;0])) then pause,end
  if or(cumprod(b,'*',typ(:))<>hypermat([2,2,2],[1;0;0;0;0;0;0;0])) then pause,end
  if or(cumprod(b,1,typ(:))<>hypermat([2,2,2],[1;0;1;1;0;0;0;0])) then pause,end
  if or(cumprod(b,2,typ(:))<>hypermat([2,2,2],[1;0;1;0;0;0;0;0])) then pause,end
  if or(cumprod(b,3,typ(:))<>hypermat([2,2,2],[1;0;1;1;0;0;0;0])) then pause,end
  if or(cumprod(b,5,typ(:))<>double(b)) then pause,end
end

if or(cumprod(b,'native')<>hypermat([2,2,2],[%t;%f;%f;%f;%f;%f;%f;%f])) then pause,end
if or(cumprod(b,'*','native')<>hypermat([2,2,2],[%t;%f;%f;%f;%f;%f;%f;%f])) then pause,end
if or(cumprod(b,1,'native')<>hypermat([2,2,2],[%t;%f;%t;%t;%f;%f;%f;%f])) then pause,end
if or(cumprod(b,2,'native')<>hypermat([2,2,2],[%t;%f;%t;%f;%f;%f;%f;%f])) then pause,end
if or(cumprod(b,3,'native')<>hypermat([2,2,2],[%t;%f;%t;%t;%f;%f;%f;%f])) then pause,end
if or(cumprod(b,5,'native')<>b) then pause,end

//=======================================================================
//sparse matrices of floats
s=sparse([1 10 0;-1 0 9]);
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumprod(s,typ(:))<>sparse([1,1;1,2;2,1],[1;-10;-1],[2,3])) then pause,end
  if or(cumprod(s,'*',typ(:))<>sparse([1,1;1,2;2,1],[1;-10;-1],[2,3])) then pause,end
  if or(cumprod(s,1,typ(:))<>sparse([1,1;1,2;2,1],[1;10;-1],[2,3])) then pause,end
  if or(cumprod(s,2,typ(:))<>sparse([1,1;1,2;2,1],[1;10;-1],[2,3])) then pause,end
  if or(cumprod(s,3,typ(:))<>s) then pause,end
end

//=======================================================================
//sparse  matrices of boolean
bs=sparse([%t %t %f;%t %t %t]);
for typ=list(list(),list('double'));
  if or(cumprod(bs,typ(:))<>sparse([1,1;1,2;2,1;2,2],[1;1;1;1],[2,3])) then pause,end
  if or(cumprod(bs,'*',typ(:))<>sparse([1,1;1,2;2,1;2,2],[1;1;1;1],[2,3])) then pause,end
  if or(cumprod(bs,1,typ(:))<>sparse([1,1;1,2;2,1;2,2],[1;1;1;1],[2,3])) then pause,end
  if or(cumprod(bs,2,typ(:))<>sparse([1,1;1,2;2,1;2,2;2,3],[1;1;1;1;1],[2,3])) then pause,end
  if or(cumprod(bs,3,typ(:))<>bool2s(bs)) then pause,end
end

if or(cumprod(bs,'native')<>sparse([1,1;1,2;2,1;2,2],[%t;%t;%t;%t],[2,3])) then pause,end
if or(cumprod(bs,'*','native')<>sparse([1,1;1,2;2,1;2,2],[%t;%t;%t;%t],[2,3])) then pause,end

if or(cumprod(bs,1,'native')<>sparse([1,1;1,2;2,1;2,2],[%t;%t;%t;%t],[2,3])) then pause,end
if or(cumprod(bs,2,'native')<>sparse([1,1;1,2;2,1;2,2;2,3],[%t;%t;%t;%t;%t],[2,3])) then pause,end
if or(cumprod(bs,3,'native')<>bs) then pause,end

// TODO : test the "m" option
