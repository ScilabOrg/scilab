//K Optimal controller , ro = gamaopt^-2;

[K,ro]=h_inf(P,r,0,1,30);

K,gamaopt=1/sqrt(ro)

// Check internal stability:
