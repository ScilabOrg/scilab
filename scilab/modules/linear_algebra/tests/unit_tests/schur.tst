// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function r=Err(x)
	r=norm(x,1)
endfunction
rand('normal')

//define tools
function A=testmat1(a,n)
	//eigen values are given by a dilation of nth roots of 1
	A=diag(a*ones(1,n-1),1)+diag((1/a)*ones(1,n-1),-1)
	A(1,n)=1/a;A(n,1)=a
endfunction

//==========================================================================
//==============================    schur     ============================== 
//==========================================================================
clear sel
function t=sel(R),t=real(R)<0 ,endfunction
//Empty matrix
A=[];
if schur(A)<>[] then pause,end
if schur(A,'real')<>[] then pause,end
if schur(A,'complex')<>[] then pause,end

if schur(A,'c')<>[] then pause,end
if schur(A,'d')<>[] then pause,end
if schur(A,sel)<>[] then pause,end

[U,S]=schur(A);
if U<>[]|S<>[] then pause,end
[U,S]=schur(A,'real');
if U<>[]|S<>[] then pause,end
[U,S]=schur(A,'complex');
if U<>[]|S<>[] then pause,end



[U,N]=schur(A,'c');
if U<>[]|N<>0 then pause,end
[U,N]=schur(A,'d');
if U<>[]|N<>0 then pause,end
[U,N]=schur(A,sel);
if U<>[]|N<>0 then pause,end

[U,N,S]=schur(A,'c');
if U<>[]|N<>0|S<>[] then pause,end
[U,N,S]=schur(A,'d');
if U<>[]|N<>0|S<>[] then pause,end
[U,N,S]=schur(A,sel);
if U<>[]|N<>0|S<>[] then pause,end

//Rectangular matrix
if execstr('schur(rand(2,3))','errcatch')==0 then pause,end
if execstr('[U,S]=schur(rand(2,3))','errcatch')==0 then pause,end

if execstr('schur(rand(2,3)+%i*eye())','errcatch')==0 then pause,end
if execstr('[U,S]=schur(rand(2,3)+%i*eye())','errcatch')==0 then pause,end

//Small dimension
A=testmat1(3,5);Ac=testmat1(3+%i,5);
//Real
[U,S]=schur(A);
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-A)>200*%eps then pause,end
if Err(schur(A)-S) >%eps then pause,end

[U,S]=schur(A,'real');
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-A)>200*%eps then pause,end
if Err(schur(A)-S) >%eps then pause,end

[U,S]=schur(A,'complex');
if Err(triu(S)-S)>%eps then pause,end
if Err(U*S*U'-A)>200*%eps then pause,end
if Err(schur(A,'complex')-S) >%eps then pause,end

[U,n]=schur(A,'c');S=U'*A*U;
if n<>2 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

[U,n]=schur(A,'d');S=U'*A*U;
if n<>0 then pause,end
if or(abs(spec(S(n+1:$,n+1:$)))<1) then pause,end

[U,n]=schur(A,sel);S=U'*A*U;
if n<>2 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end


//Complex
[U,S]=schur(Ac);
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-Ac)>200*%eps then pause,end
if Err(schur(Ac)-S) >%eps then pause,end

[U,S]=schur(Ac,'complex');
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-Ac)>200*%eps then pause,end
if Err(schur(Ac)-S) >%eps then pause,end


[U,n]=schur(Ac,'c');S=U'*Ac*U;
if n<>3 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

[U,n]=schur(Ac,'d');S=U'*A*U;
if n<>0 then pause,end
if or(abs(spec(S(n+1:$,n+1:$)))<1) then pause,end

[U,n]=schur(Ac,sel);S=U'*Ac*U;
if n<>3 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end


//Large dimension
A=testmat1(3,50);Ac=testmat1(3+%i,50);
//Real
[U,S]=schur(A);
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-A)>1000*%eps then pause,end
if Err(schur(A)-S) >%eps then pause,end

[U,S]=schur(A,'real');
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-A)>1000*%eps then pause,end
if Err(schur(A)-S) >%eps then pause,end

[U,S]=schur(A,'complex');
if Err(triu(S)-S)>%eps then pause,end
if Err(U*S*U'-A)>1000*%eps then pause,end
if Err(schur(A,'complex')-S) >%eps then pause,end


[U,n]=schur(A,'c');S=U'*A*U;
if n<>25 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

[U,n]=schur(A,'d');S=U'*A*U;
if n<>0 then pause,end
if or(abs(spec(S(n+1:$,n+1:$)))<1) then pause,end

[U,n]=schur(A,sel);S=U'*A*U;
if n<>25 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

//Complex
[U,S]=schur(Ac);
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-Ac)>1000*%eps then pause,end
if Err(schur(Ac)-S) >%eps then pause,end

[U,S]=schur(Ac,'complex');
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-Ac)>1000*%eps then pause,end
if Err(schur(Ac)-S) >%eps then pause,end

[U,n]=schur(Ac,'c');S=U'*Ac*U;
if n<>25 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

[U,n]=schur(Ac,'d');S=U'*Ac*U;
if n<>0 then pause,end
if or(abs(spec(S(n+1:$,n+1:$)))<1) then pause,end

[U,n]=schur(Ac,sel);S=U'*Ac*U;
if n<>25 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end
//==========================================================================
//==============================    schur part II   ======================== 
//==========================================================================
//Empty matrix
[As,Es]=schur([],[]);
if As<>[]|Es<>[] then pause,end

[As,dim]=schur([],[],'c');
if As<>[]|dim<>0 then pause,end
[As,dim]=schur([],[],'d');
if As<>[]|dim<>0 then pause,end
[As,dim]=schur([],[],sel);
if As<>[]|dim<>0 then pause,end

[As,Es,Q,Z]=schur([],[]);
if As<>[]|Es<>[]|Q<>[]|Z<>[] then pause,end

[As,Es,dim]=schur([],[],'c');
if As<>[]|Es<>[]|dim<>0 then pause,end
[As,Es,dim]=schur([],[],'d');
if As<>[]|Es<>[]|dim<>0 then pause,end
[As,Es,dim]=schur([],[],sel);
if As<>[]|Es<>[]|dim<>0 then pause,end

[Z,dim]=schur([],[],'c');
if Z<>[]|dim<>0 then pause,end
[Z,dim]=schur([],[],'d');
if Z<>[]|dim<>0 then pause,end
[Z,dim]=schur([],[],sel);
if Z<>[]|dim<>0 then pause,end


//Rectangular matrix
if execstr('[As,Es]=schur(rand(2,3),rand(2,3))','errcatch')==0 then  pause,end
if execstr('[As,Es,Q,Z]=schur(rand(2,3),rand(2,3))','errcatch')==0 then  pause,end
if execstr('[As,Es,dim]=schur(rand(2,3),rand(2,3),''c'')','errcatch')==0 then  pause,end
if execstr('[Z,dim]=schur(rand(2,3),rand(2,3),sel)','errcatch')==0 then  pause,end

//Small dimension
//----Real------------
A=testmat1(1,5);E=testmat1(-2,5) ;
[As,Es,Q,Z]=schur(A,E);
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

[As1,Es1]=schur(A,E);
if Err(As1-As)>10*%eps then pause,end
if Err(Es1-Es)>10*%eps then pause,end

// Ordered 'c'
dim=schur(A,E,'c');
if dim<>5 then pause,end
[Z,dim]=schur(A,E,'c');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'c');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>5 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>5 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
// Ordered 'd'
dim=schur(A,E,'d');
if dim<>5 then pause,end
[Z,dim]=schur(A,E,'d');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'d');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>5 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>5 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

//ordered sel
clear sel
function t=sel(Alpha,Beta),t=real(Alpha)>-0.2*real(Beta) ,endfunction

dim=schur(A,E,sel);
if dim<>2 then pause,end
[Z,dim]=schur(A,E,sel);
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,sel);
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>2 then pause,end

[As,Es,Z,dim]=schur(A,E,sel);
if dim<>2 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
//----Complex------------
A=testmat1(1+%i,5);E=testmat1(-2-3*%i,5) ;
[As,Es,Q,Z]=schur(A,E);
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

[As1,Es1]=schur(A,E);
if Err(As1-As)>10*%eps then pause,end
if Err(Es1-Es)>10*%eps then pause,end

// Ordered 'c'
dim=schur(A,E,'c');
if dim<>5 then pause,end
[Z,dim]=schur(A,E,'c');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'c');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>5 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>5 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
// Ordered 'd'
dim=schur(A,E,'d');
if dim<>5 then pause,end
[Z,dim]=schur(A,E,'d');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'d');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>5 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>5 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

//ordered sel
clear sel
function t=sel(Alpha,Beta),t=imag(Alpha)>0 ,endfunction

dim=schur(A,E,sel);
if dim<>3 then pause,end
[Z,dim]=schur(A,E,sel);
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,sel);
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>3 then pause,end

[As,Es,Z,dim]=schur(A,E,sel);
if dim<>3 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

//Large dimension

//----Real------------
A=testmat1(1,50);E=testmat1(-2,50) ;
[As,Es,Q,Z]=schur(A,E);
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

[As1,Es1]=schur(A,E);
if Err(As1-As)>10*%eps then pause,end
if Err(Es1-Es)>10*%eps then pause,end

// Ordered 'c'
dim=schur(A,E,'c');
if dim<>50 then pause,end
[Z,dim]=schur(A,E,'c');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'c');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>50 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>50 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
// Ordered 'd'
dim=schur(A,E,'d');
if dim<>50 then pause,end
[Z,dim]=schur(A,E,'d');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'d');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>50 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>50 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

//ordered sel
clear sel
function t=sel(Alpha,Beta)
	t=real(Alpha)>-0.2*real(Beta)
endfunction

dim=schur(A,E,sel); // plante ici DGGES LAPACK 3.1
if dim<>12 then pause,end
[Z,dim]=schur(A,E,sel);
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,sel);
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>12 then pause,end

[As,Es,Z,dim]=schur(A,E,sel);
if dim<>12 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
//----Complex------------
A=testmat1(1+%i,50);E=testmat1(-2-3*%i,50) ;
[As,Es,Q,Z]=schur(A,E);
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >1000*%eps then pause,end
if Err(Es-Q'*E*Z) >1000*%eps then pause,end

[As1,Es1]=schur(A,E);
if Err(As1-As)>10*%eps then pause,end
if Err(Es1-Es)>10*%eps then pause,end

// Ordered 'c'
dim=schur(A,E,'c');
if dim<>50 then pause,end
[Z,dim]=schur(A,E,'c');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'c');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>50 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>50 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >1000*%eps then pause,end
if Err(Es-Q'*E*Z) >1000*%eps then pause,end
// Ordered 'd'
dim=schur(A,E,'d');
if dim<>50 then pause,end
[Z,dim]=schur(A,E,'d');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'d');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>50 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>50 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >1000*%eps then pause,end
if Err(Es-Q'*E*Z) >1000*%eps then pause,end

//ordered sel
clear sel
function t=sel(Alpha,Beta),t=imag(Alpha)>0 ,endfunction

dim=schur(A,E,sel);
if dim<>32 then pause,end
[Z,dim]=schur(A,E,sel);
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,sel);
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>32 then pause,end

[As,Es,Z,dim]=schur(A,E,sel);
if dim<>32 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >1000*%eps then pause,end
if Err(Es-Q'*E*Z) >1000*%eps then pause,end

