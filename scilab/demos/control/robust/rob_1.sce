// Copyright INRIA
clc;
mode(1)

s=poly(0,'s');

//MAC-FARLANE PROBLEM for G=1/s^3;

[P,r]=macglov(1/s^3);clean(P)

//Optimal controller K:

halt()
//K Optimal controller , ro = gamaopt^-2;
[K,ro]=h_inf(P,r,0,1,30);
K,gamaopt=1/sqrt(ro)

// Check internal stability:

halt()

Tzw=lft(tf2ss(P),tf2ss(K));

[Acl,Bcl,Ccl,Dcl]=abcd(Tzw); spec(Acl)

//Optimal gain:

halt()

ga=h_norm(Tzw)

//Compare with gamaopt

ga-gamaopt

//Compare with theory

halt()

[N,M]=lcf(tf2ss(1/s^3)); //Left coprime factorization of G.

nk=hankelsv([N,M]);

ro-( 1-nk(1) )



  	
