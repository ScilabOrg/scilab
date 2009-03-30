// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - ENPC
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// test of fscanfMat and fprintfMat
// --------------------------------
n=50;
a=rand(n,n,'u');
// now the data 
fd=mopen(TMPDIR+'/Mat','w');
texte=['Some text ';'Some more text'];
for t=texte 
  mfprintf(fd,'%s\n',t);
end 
for i=1:n ,
  for j=1:n, mfprintf(fd,'%5.2f ',a(i,j));end;
  mfprintf(fd,'\n');	
end
mclose(fd);
a1=fscanfMat(TMPDIR+'/Mat');
if maxi(a1-a) > 1.e-1 then pause,end 

[a1,txt]=fscanfMat(TMPDIR+'/Mat');
if ~and(txt==texte) then pause,end 
if maxi(a1-a) > 1.e-1 then pause,end 

// ---- test with fprintfMat 
n=50;
a=rand(n,n,'u');
fprintfMat(TMPDIR+'/Mat',a,'%5.2f');
a1=fscanfMat(TMPDIR+'/Mat');
if maxi(a1-a) > 1.e-1 then pause,end 

// ---- test with fprintfMat 
n=50;
a=rand(n,n,'u');
fprintfMat(TMPDIR+'/Mat',a,'%5.2f',txt);
[a1,txt1]=fscanfMat(TMPDIR+'/Mat');
if maxi(a1-a) > 1.e-1 then pause,end 
if ~and(txt==txt1) then pause,end 

n=100;
a=rand(n,n,'u');
fprintfMat(TMPDIR+'/Mat',a)
[a1]=fscanfMat(TMPDIR+'/Mat');
if maxi(a1-a) > 1.e-1 then pause,end 
[a1,S]=fscanfMat(TMPDIR+'/Mat');
if S<>emptystr() then pause,end 





