// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function r=Err(x)
  r=norm(x,1)
endfunction
rand('normal')

//==========================================================================
//==============================     svd      ============================== 
//==========================================================================
//Empty matrix
A=[];
[U,S,V]=svd(A);
if U<>[]|V<>[]|S<>[] then pause,end
S=svd(A);
if S<>[] then pause,end
[U,S,V]=svd(A,"e");
if U<>[]|V<>[]|S<>[] then pause,end

S=svd(A,"e");
if S<>[] then pause,end

//Matrix with inf or nan
if execstr('svd([%inf 1;2 3])','errcatch')==0 then pause,end
if execstr('svd([1 %nan;2 3])','errcatch')==0 then pause,end

if execstr('svd([%inf %i;2 3])','errcatch')==0 then pause,end
if execstr('svd([%i %i;%nan 3])','errcatch')==0 then pause,end


//Small dimension
//---------------
A=rand(3,5);
Ac=A+%i*rand(A);

//Real Case
[U,S,V]=svd(A);
if Err(U*S*V'-A)>200*%eps then pause,end
if Err(svd(A)-diag(S))> 200*%eps then pause,end

[U,S,V]=svd(A,"e");
if Err(U*S*V'-A)>200*%eps then pause,end

A=A';
[U,S,V]=svd(A);
if Err(U*S*V'-A)>200*%eps then pause,end
if Err(svd(A)-diag(S))> 200*%eps then pause,end

[U,S,V]=svd(A,"e");
if Err(U*S*V'-A)>200*%eps then pause,end


//Complex Case
[U,S,V]=svd(Ac);
if Err(U*S*V'-Ac)>200*%eps then pause,end
if Err(svd(Ac)-diag(S))> 200*%eps then pause,end

[U,S,V]=svd(Ac,"e");
if Err(U*S*V'-Ac)>200*%eps then pause,end

Ac=Ac';
[U,S,V]=svd(Ac);U*S*V'-Ac;
if Err(U*S*V'-Ac)>200*%eps then pause,end
if Err(svd(Ac)-diag(S))> 200*%eps then pause,end

[U,S,V]=svd(Ac,"e");
if Err(U*S*V'-Ac)>200*%eps then pause,end


//Large dimension
//---------------
A=rand(150,60);
Ac=A+rand(A)*%i;
//Real Case
[U,S,V]=svd(A);
if Err(U*S*V'-A)>10000*%eps then pause,end
if Err(svd(A)-diag(S))> 10000*%eps then pause,end

[U,S,V]=svd(A,"e");
if Err(U*S*V'-A)>10000*%eps then pause,end

A=A';
[U,S,V]=svd(A);
if Err(U*S*V'-A)>10000*%eps then pause,end
if Err(svd(A)-diag(S))> 10000*%eps then pause,end

[U,S,V]=svd(A,"e");
if Err(U*S*V'-A)>10000*%eps then pause,end


//Complex Case
[U,S,V]=svd(Ac);
if Err(U*S*V'-Ac)>10000*%eps then pause,end
if Err(svd(Ac)-diag(S))> 10000*%eps then pause,end

[U,S,V]=svd(Ac,"e");
if Err(U*S*V'-Ac)>10000*%eps then pause,end

Ac=Ac';
[U,S,V]=svd(Ac);U*S*V'-Ac;
if Err(U*S*V'-Ac)>10000*%eps then pause,end
if Err(svd(Ac)-diag(S))> 10000*%eps then pause,end

[U,S,V]=svd(Ac,"e");
if Err(U*S*V'-Ac)>10000*%eps then pause,end

//==========================================================================
//==============================     svd part II     ======================= 
//==========================================================================
//Empty matrix
if svd([])<>[] then pause,end
if svd([],"e")<>[] then pause,end

[U,S]=svd([]);
if U<>[]|S<>[]  then pause,end
[U,S,V]=svd([]);
if U<>[]|S<>[]|V<>[]  then pause,end
[U,S,V,rk]=svd([]);
if U<>[]|S<>[]|V<>[]|rk<>0  then pause,end
[U,S,V,rk]=svd([],%eps);
if U<>[]|S<>[]|V<>[]|rk<>0  then pause,end

if execstr('[U,S,V,rk]=svd([],'"e'")','errcatch') == 0 then pause,end

//Small dimension
//Real Case Fat
A=rand(3,5);
S=svd(A);
if or(S<0) then pause,end
if gsort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>10*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U1'*U1-eye())>200*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end
if Err(U1*S1*V'-A) >200*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>10*%eps  then pause,end
if Err(U1*S1*V1'-A) >200*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(V-V1) >200*%eps  then pause,end
if rk<>3 then pause,end

//Real Case Tall
A=rand(5,3);

S=svd(A);
if or(S<0) then pause,end
if gsort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U1'*U1-eye())>200*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end
if Err(U1*S1*V'-A) >200*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if size(U1,2)<>3 then pause,end
if Err(U1*S1*V1'-A) >200*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(V-V1) >200*%eps  then pause,end
if rk<>3 then pause,end

//Complex Case Fat
A=rand(3,5)+%i*rand(3,5);
S=svd(A);
if or(S<0) then pause,end
if gsort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U1'*U1-eye())>200*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end
if Err(U1*S1*V'-A) >30*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(U1*S1*V1'-A) >200*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(V-V1) >200*%eps  then pause,end
if rk<>3 then pause,end

//Complex Case Tall
A=rand(5,3)+%i*rand(5,3);

S=svd(A);
if or(S<0) then pause,end
if gsort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U1'*U1-eye())>200*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end
if Err(U1*S1*V'-A) >200*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if size(U1,2)<>3 then pause,end
if Err(U1*S1*V1'-A) >200*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(V-V1) >200*%eps  then pause,end
if rk<>3 then pause,end


//Large dimension
//Real Case Fat
A=rand(30,50);
S=svd(A);
if or(S<0) then pause,end
if gsort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U'*U-eye())>1000*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U1'*U1-eye())>1000*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U'*U-eye())>1000*%eps  then pause,end
if Err(U1*S1*V'-A) >1000*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U-U1)>10*%eps  then pause,end
if Err(U1*S1*V1'-A) >1000*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(V-V1) >5000*%eps  then pause,end
if rk<>30 then pause,end

//Real Case Tall
A=rand(50,30);

S=svd(A);
if or(S<0) then pause,end
if gsort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U1'*U1-eye())>5000*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end
if Err(U1*S1*V'-A) >5000*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if size(U1,2)<>30 then pause,end
if Err(U1*S1*V1'-A) >5000*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(V-V1) >5000*%eps  then pause,end
if rk<>30 then pause,end

//Complex Case Fat
A=rand(30,50)+%i*rand(30,50);
S=svd(A);
if or(S<0) then pause,end
if gsort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U1'*U1-eye())>5000*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end
if Err(U1*S1*V'-A) >5000*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(U1*S1*V1'-A) >5000*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(V-V1) >5000*%eps  then pause,end
if rk<>30 then pause,end

//Complex Case Tall
A=rand(50,30)+%i*rand(50,30);

S=svd(A);
if or(S<0) then pause,end
if gsort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U1'*U1-eye())>5000*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end
if Err(U1*S1*V'-A) >5000*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if size(U1,2)<>30 then pause,end
if Err(U1*S1*V1'-A) >5000*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(V-V1) >5000*%eps  then pause,end
if rk<>30 then pause,end


