// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI TEST -->
// prod 
//empty matrices
T=list(list(),list('native'),list('double'));
for typ=T
  if prod([],typ(:))<>1 then pause,end
  if prod([],'*',typ(:))<>1 then pause,end
  if prod([],1,typ(:))<>[] then pause,end
  if prod([],2,typ(:))<>[] then pause,end
  if prod([],3,typ(:))<>[] then pause,end
end

//=======================================================================
//float matrices
d=[1 10;254 9];
T=list(list(),list('native'),list('double'));
for typ=T
  if prod(d,typ(:))<>22860 then pause,end
  if prod(d,'*',typ(:))<>22860 then pause,end
  if or(prod(d,1,typ(:))<>[254 90]) then pause,end
  if or(prod(d,2,typ(:))<>[10;2286]) then pause,end
  if or(prod(d,3,typ(:))<>d) then pause,end
end


//hypermatrices of floats
d=[1 10;254 9];d(:,:,2)=1;
T=list(list(),list('native'),list('double'));
for typ=T
  if prod(d,typ(:))<>22860 then pause,end
  if prod(d,'*',typ(:))<>22860 then pause,end
  if or(prod(d,1,typ(:))<>hypermat([1,2,2],[254;90;1;1])) then pause,end
  if or(prod(d,2,typ(:))<>hypermat([2,1,2],[10;2286;1;1])) then pause,end
  if or(prod(d,3,typ(:))<>[1,10;254,9]) then pause,end
  if or(prod(d,5,typ(:))<>d) then pause,end
end

//=======================================================================
//matrices of short integers
i=uint8([1 10;254 9]);
T=list(list(),list('native'));
for typ=T
  if prod(i,typ(:))<>uint8(76) then pause,end
  if prod(i,'*',typ(:))<>uint8(76) then pause,end
  if or(prod(i,1,typ(:))<>uint8([254 90])) then pause,end
  if or(prod(i,2,typ(:))<>uint8([10;238])) then pause,end
  if or(prod(i,3,typ(:))<>i) then pause,end
end

if prod(i,'double')<>22860 then pause,end
if prod(i,'*','double')<>22860 then pause,end
if or(prod(i,1,'double')<>[254 90]) then pause,end
if or(prod(i,2,'double')<>[10;2286]) then pause,end
if or(prod(i,3,'double')<>double(i)) then pause,end

//with hypermatrices
i=uint8([1 10;254 9]);i(:,:,2)=uint8(1);
for typ=list(list(),list('native'));
  if prod(i,typ(:))<>uint8(76) then pause,end
  if prod(i,'*',typ(:))<>uint8(76) then pause,end
  if or(prod(i,1,typ(:))<>hypermat([1,2,2],uint8([254;90;1;1]))) then pause,end
  if or(prod(i,2,typ(:))<>hypermat([2,1,2],uint8([10;238;1;1]))) then pause,end
  if or(prod(i,3,typ(:))<>([1,10;254,9])) then pause,end
  if or(prod(i,5,typ(:))<>i) then pause,end
end

if prod(i,'double')<>22860 then pause,end
if prod(i,'*','double')<>22860 then pause,end
if or(prod(i,1,'double')<>hypermat([1,2,2],[254;90;1;1])) then pause,end
if or(prod(i,2,'double')<>hypermat([2,1,2],[10;2286;1;1])) then pause,end
if or(prod(i,3,'double')<>[1,10;254,9]) then pause,end
if or(prod(i,5,'double')<>double(i)) then pause,end


//Polynomials
s=%s;p=[s s+1;s^2 1];
T=list(list(),list('native'),list('double'));
for typ=T
  if prod(p,typ(:))<>s^3+s^4 then pause,end
  if prod(p,'*',typ(:))<>s^3+s^4 then pause,end
  if or(prod(p,1,typ(:))<>[s^3,1+s]) then pause,end
  if or(prod(p,2,typ(:))<>[s+s^2;s^2]) then pause,end
  if or(prod(p,3,typ(:))<>p) then pause,end
end
//with hypermatrices
s=%s;p=[s s+1;s^2 1];p(:,:,2)=[1 s;s+1 2];
T=list(list(),list('native'),list('double'));
for typ=T
  if prod(p,typ(:))<>2*s^4+4*s^5+2*s^6 then pause,end
  if prod(p,'*',typ(:))<>2*s^4+4*s^5+2*s^6 then pause,end
  if or(prod(p,1,typ(:))<>hypermat([1,2,2],[s^3;1+s;1+s;2*s])) then pause,end
  if or(prod(p,2,typ(:))<>hypermat([2,1,2],[s+s^2;s^2;s;2+2*s])) then pause,end
  if or(prod(p,3,typ(:))<>[s,s+s^2;s^2+s^3,2]) then pause,end
  if or(prod(p,5,typ(:))<>p) then pause,end
end

//=======================================================================
//Matrices of rationals
s=%s;r=1.0 ./[s s+1;s^2 1];
T=list(list(),list('native'),list('double'));
for typ=T
  if prod(r,typ(:))<>1.0/(s^3+s^4) then pause,end
  if prod(r,'*',typ(:))<>1.0/(s^3+s^4) then pause,end
  if or(prod(r,1,typ(:))<>[1,1]./[s^3,1+s]) then pause,end
  if or(prod(r,2,typ(:))<>[1;1]./[s+s^2;s^2]) then pause,end
  if or(prod(r,3,typ(:))<>r) then pause,end
end

//=======================================================================
//Matrices of booleans
b=[%t %t; %f %t];
for typ=list(list(),list('double'));
  if prod(b,typ(:))<>0 then pause,end
  if prod(b,'*',typ(:))<>0 then pause,end
  if or(prod(b,1,typ(:))<>[0 1]) then pause,end
  if or(prod(b,2,typ(:))<>[1;0]) then pause,end
  if or(prod(b,3,typ(:))<>double(b)) then pause,end
end

if prod(b,'native')<>%f then pause,end
if prod(b,'*','native')<>%f then pause,end
if or(prod(b,1,'native')<>[%f %t]) then pause,end
if or(prod(b,2,'native')<>[%t;%f]) then pause,end
if or(prod(b,3,'native')<>b) then pause,end
//with hypermatrices 
  
b=[%t %t;%f %t];b(1,1,2)=%t;
for typ=list(list(),list('double'));
  if prod(b,typ(:))<>0 then pause,end
  if prod(b,'*',typ(:))<>0 then pause,end
  if or(prod(b,1,typ(:))<>hypermat([1,2,2],[0;1;0;0])) then pause,end
  if or(prod(b,2,typ(:))<>hypermat([2,1,2],[1;0;0;0])) then pause,end
  if or(prod(b,3,typ(:))<>[1,0;0,0]) then pause,end
  if or(prod(b,5,typ(:))<>double(b)) then pause,end
end

if prod(b,'native')<>%f then pause,end
if prod(b,'*','native')<>%f then pause,end
if or(prod(b,1,'native')<>hypermat([1,2,2],[%f;%t;%f;%f])) then pause,end
if or(prod(b,2,'native')<>hypermat([2,1,2],[%t;%f;%f;%f])) then pause,end
if or(prod(b,3,'native')<>[%t,%f;%f,%f]) then pause,end
if or(prod(b,5,'native')<>b) then pause,end
//=======================================================================
//sparse matrices of floats
s=sparse([1 10 2;-1 254 9]);
T=list(list(),list('native'),list('double'));
for typ=T
  if prod(s,typ(:))<>-45720 then pause,end
  if prod(s,'*',typ(:))<>-45720 then pause,end
  if or(prod(s,1,typ(:))<>sparse([-1 2540 18])) then pause,end
  if or(prod(s,2,typ(:))<>sparse([20;-2286])) then pause,end
  if or(prod(s,3,typ(:))<>s) then pause,end
end

//=======================================================================
//sparse  matrices of boolean
bs=sparse([%t %t %f;%t %t %t]);
for typ=list(list(),list('double'));
  if prod(bs,typ(:))<>0 then pause,end
  if prod(bs,'*',typ(:))<>0 then pause,end
  if or(prod(bs,1,typ(:))<>sparse([1,1,0])) then pause,end
  if or(prod(bs,2,typ(:))<>sparse([0;1])) then pause,end
  if or(prod(bs,3,typ(:))<>bool2s(bs)) then pause,end
end

if prod(bs,'native')<>%f then pause,end
if prod(bs,'*','native')<>%f then pause,end
if or(prod(bs,1,'native')<>sparse([%t,%t,%f])) then pause,end
if or(prod(bs,2,'native')<>sparse([%f;%t])) then pause,end
if or(prod(bs,3,'native')<>bs) then pause,end

// TODO : test the "m" option
