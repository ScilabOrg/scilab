// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for eigs function 
// =============================================================================

// Interface
// =========
assert_checkfalse(execstr("eigs()"   ,"errcatch") == 0);
assert_checkfalse(execstr("eigs(1)","errcatch") == 0);
assert_checkfalse(execstr("eigs([])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(%nan)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(%inf)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(%eps)", "errcatch") == 0);

assert_checkfalse(execstr("eigs([%f %f])", "errcatch") == 0);
assert_checkfalse(execstr("eigs([%f %f])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse([%f %f]))", "errcatch") == 0);
assert_checkfalse(execstr("eigs([1 2])", "errcatch") == 0);
assert_checkfalse(execstr("eigs([1; 2])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse([1 2]))", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse([1; 2]))", "errcatch") == 0);

n = 20;
A            = diag(10*ones(n,1));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6*ones(n-1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(6*ones(n-1,1));

assert_checkfalse(execstr("eigs(A, %f)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, %nan)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, %inf)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, %eps)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), %f)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), %nan)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), %inf)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), %eps)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A,[1 2])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A,[1;2])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [1 2])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [1;2])", "errcatch") == 0);

assert_checkfalse(execstr("eigs(A, [], [])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], %f)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], 2*%i)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], -15)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], 5.6)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], [1 2])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], %nan)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], %eps)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], %inf)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], %f)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 2*%i)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], -15)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 5.6)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], [1 2])", "errcatch") == 0);

assert_checkfalse(execstr("eigs(A, [], 4, [])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], 4, %nan)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(A, [], 4, %f)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, [])", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, %nan)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, %f)", "errcatch") == 0);
which = 'ahh';
assert_checkfalse(execstr("eigs(A, [], 4, which)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which)", "errcatch") == 0);

which = 'LM';
assert_checkfalse(execstr("eigs(A, [], 4, which ,%nan)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,%nan)", "errcatch") == 0);
opts.var = %nan;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,optsn)", "errcatch") == 0);

clear opts
opts.maxiter  = [];
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.maxiter  = %nan;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.maxiter  = %f;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.maxiter  = "ahh";
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.maxiter = 5.1;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.maxiter = -5.1;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);

clear opts
opts.tol  = [];
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.tol  = %nan;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.tol  = %f;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.tol  = "ahh";
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);

clear opts
opts.ncv  = %nan;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.ncv  = %f;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.ncv  = "ahh";
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.ncv  = %eps;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.ncv  = -5.1;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.ncv  = 5.1;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.ncv = n + 6;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);

clear opts
opts.cholB  = %nan;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.cholB  = %f;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.cholB  = "ahh";
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.cholB  = %eps;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.cholB  = -5.1;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.cholB  = 5.1;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.cholB = [];
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);

clear opts
opts.resid  = %nan;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.resid  = %f;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.resid  = "ahh";
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.resid  = %eps;
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.resid  = [1 2];
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.resid  = [1;2];
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);
opts.resid = [];
assert_checkfalse(execstr("eigs(A, [], 4, which ,opts)", "errcatch") == 0);
assert_checkfalse(execstr("eigs(sparse(A), [], 4, which ,opts)", "errcatch") == 0);

clear opts
n = 20;
k = 4;
A            = diag(10*ones(n,1));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6*ones(n-1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(6*ones(n-1,1));
A = sparse(A);

d1 = eigs(A, [], k);
d0 = spec(full(A));
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

d1 = eigs(A, [], k+1);
assert_checkalmostequal(d1, d0(($-k):$), 1.e-10);

d1 = eigs(A, [], k, 'LM');
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

d1 = eigs(A, [], k, 'SM');
assert_checkalmostequal(d1, d0(2:k+1), 1.e-10);

d1 = eigs(A, [], k, 'LA');
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

d1 = eigs(A, [], k, 'SA');
assert_checkalmostequal(d1, d0(k:-1:1), 1.e-10);

d1 = eigs(A, [], k, 'BE');
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

d1 = eigs(A, [], k, 4.1); 
assert_checkalmostequal(d1, d0(5:5+k-1), 1.e-10);

d1 = eigs(A, speye(n,n), k, 'LM');
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

assert_checkalmostequal(eigs(A, [],k, 4.1), eigs(A, speye(n,n), k, 4.1), 1.e-10);

opts.cholB = 1;
d1 = eigs(A, speye(n,n), k, 'LM', opts); 
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

opts.cholB = 1;
d1 = eigs(A, speye(n,n), k, 4.1, opts); 
assert_checkalmostequal(eigs(A, [],k, 4.1), d1, 1.e-10);

[d1, v1] = eigs(A, [], k, 'LM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LA');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SA');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'BE');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);


n = 20;
k = 4;
A            = diag((1:n));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6*ones(n-1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(-6*ones(n-1,1));
A = sparse(A);

d1 = eigs(A, [], k);
d0 = gsort(spec(full(A)));

assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10); 

d1 = eigs(A, [], k+1);
assert_checkalmostequal(abs(d1), abs(d0(k+2:-1:2)), 1.e-10); 

d1 = eigs(A, [], k, 'LM');
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

d1 = eigs(A, [], k, 'SM');
assert_checkalmostequal(abs(d1), abs(d0($-k+1:$)), 1.e-10);

d1 = eigs(A, [], k, 'LR');
assert_checkalmostequal(real(d1), real(d0(k:-1:1)), 1.e-10);

d1 = eigs(A, [], k, 'SR');
assert_checkalmostequal(real(d1), real(d0($:-1:$-k+1)), 1.e-10);

d1 = eigs(A, [], k, 'LI');
assert_checkalmostequal(gsort(imag(d1)), gsort(imag(d0([1 2 17 18]))), 1.e-10);

d1 = eigs(A, [], k, 'SI');
assert_checkalmostequal(gsort(imag(d1)), gsort(imag(d0([11 12 15 16]))), 1.e-10);

d1 = eigs(A, [], k, 4.1); 
assert_checkalmostequal(abs(d1), abs(d0([11 12 15 16])), 1.e-10);

d1 = eigs(A, speye(n,n), k, 'LM');
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

opts.cholB = 1;
d1 = eigs(A, speye(n,n), k, 'LM', opts);
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

opts.cholB = 1;
d1 = eigs(A, speye(n,n), k, 4.1, opts);
assert_checkalmostequal(abs(eigs(A, [],k, 4.1)), abs(d1), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LR');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SR');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LI');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SI');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

n = 20;
k = 4;
A            = diag((1:n) + %i * ones(1,n));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6*ones(n-1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(-6*ones(n-1,1));
A = sparse(A);

d1 = eigs(A, [], k);
d0 = gsort(spec(full(A)));
r = gsort(real(d0));
im = gsort(imag(d0));
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

d1 = eigs(A, [], k+1);
assert_checkalmostequal(abs(d1), abs(d0(k + 1:-1:1)), 1.e-10);

d1 = eigs(A, [], k, 'LM');
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10); 

//d1 = eigs(A, [], k, 'SM');
//assert_checkalmostequal(abs(d1), abs(d0(1:k)), 1.e-14); // error -> impossible to invert complex sparse matrix

d1 = eigs(A, [], k, 'LR');
assert_checkalmostequal(real(d1), real(d0(k:-1:1)), 1.e-10);

d1 = eigs(A, [], k, 'SR');
assert_checkalmostequal(real(d1), r($-k+1:$), 1.e-10);

d1 = eigs(A, [], k, 'LI');
assert_checkalmostequal(imag(d1), im(k:-1:1), 1.e-10); 

d1 = eigs(A, [], k, 'SI');
assert_checkalmostequal(imag(d1), im($-k+1:$), 1.e-10); 

d1 = eigs(A, speye(n,n), k, 'LM');
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10); 

opts.cholB = 1;
d1 = eigs(A, speye(n,n), k, 'LM', opts);
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10); 

[d1, v1] = eigs(A, [], k, 'LM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LR');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SR');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LI');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SI');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);


n = 20;
k = 4;
A            = diag(10*ones(n,1));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6*ones(n-1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(6*ones(n-1,1));

d1 = eigs(A, [], k);
d0 = spec(A);
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

d1 = eigs(A, [], k+1);
assert_checkalmostequal(d1, d0(($-k):$), 1.e-10);

d1 = eigs(A, [], k, 'LM');
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

d1 = eigs(A, [], k, 'SM');
assert_checkalmostequal(d1, d0(2:k+1), 1.e-10);

d1 = eigs(A, [], k, 'LA');
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

d1 = eigs(A, [], k, 'SA');
assert_checkalmostequal(d1, d0(k:-1:1), 1.e-10);

d1 = eigs(A, [], k, 'BE');
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

d1 = eigs(A, [], k, 4.1); 
assert_checkalmostequal(d1, d0(5:5+k-1), 1.e-10);

d1 = eigs(A, eye(n,n), k, 'LM');
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

assert_checkalmostequal(eigs(A, [],k, 4.1), eigs(A, eye(n,n), k, 4.1), 1.e-10);

opts.cholB = 1;
d1 = eigs(A, eye(n,n), k, 'LM', opts);
assert_checkalmostequal(d1, d0(($-k+1):$), 1.e-10);

opts.cholB = 1;
d1 = eigs(A, eye(n,n), k, 4.1, opts);
assert_checkalmostequal(eigs(A, [],k, 4.1), d1, 1.e-10);

[d1, v1] = eigs(A, [], k, 'LM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LA');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SA');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'BE');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

n = 20;
k = 4;
A            = diag((1:n));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6*ones(n-1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(-6*ones(n-1,1));

d1 = eigs(A, [], k);
d0 = gsort(spec(A));

assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

d1 = eigs(A, [], k+1);
assert_checkalmostequal(abs(d1), abs(d0(k+2:-1:2)), 1.e-10);

d1 = eigs(A, [], k, 'LM');
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

d1 = eigs(A, [], k, 'SM');
assert_checkalmostequal(abs(d1), abs(d0($-k+1:$)), 1.e-10);

d1 = eigs(A, [], k, 'LR');
assert_checkalmostequal(real(d1), real(d0(k:-1:1)), 1.e-10);

d1 = eigs(A, [], k, 'SR');
assert_checkalmostequal(real(d1), real(d0($:-1:$-k+1)), 1.e-10); 

d1 = eigs(A, [], k, 'LI');
assert_checkalmostequal(gsort(imag(d1)), gsort(imag(d0([1 2 17 18]))), 1.e-10);

d1 = eigs(A, [], k, 'SI');
assert_checkalmostequal(gsort(imag(d1)), gsort(imag(d0([11 12  15 16]))), 1.e-10);

d1 = eigs(A, [], k, 4.1); 
assert_checkalmostequal(abs(d1), abs(d0([12 11 16 15])), 1.e-10);

d1 = eigs(A, eye(n,n), k, 'LM');
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

opts.cholB = 1;
d1 = eigs(A, eye(n,n), k, 'LM', opts);
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10); 

opts.cholB = 1;
d1 = eigs(A, eye(n,n), k, 4.1, opts);
assert_checkalmostequal(abs(eigs(A, [],k, 4.1)), abs(d1), 1.e-10); 

[d1, v1] = eigs(A, [], k, 'LM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LR');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SR');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LI');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SI');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);


n = 20;
k = 4;
A            = diag((1:n) + %i * ones(1,n));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6*ones(n-1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(-6*ones(n-1,1));

d1 = eigs(A, [], k);
d0 = gsort(spec(A));
r = gsort(real(d0));
im = gsort(imag(d0));

assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

d1 = eigs(A, [], k+1);
assert_checkalmostequal(abs(d1), abs(d0(k + 1:-1:1)), 1.e-10);

d1 = eigs(A, [], k, 'LM');
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

d1 = eigs(A, [], k, 'SM');
assert_checkalmostequal(abs(d1), abs(d0($-k+1:$)), 1.e-10); 

d1 = eigs(A, [], k, 'LR');
assert_checkalmostequal(real(d1), r(k:-1:1), 1.e-10);

d1 = eigs(A, [], k, 'SR');
assert_checkalmostequal(real(d1), r($-k+1:$), 1.e-10);  

d1 = eigs(A, [], k, 'LI');
assert_checkalmostequal(imag(d1), im(k:-1:1), 1.e-10); 

d1 = eigs(A, [], k, 'SI');
assert_checkalmostequal(imag(d1), im($-k+1:$), 1.e-10);

d1 = eigs(A, [], k, 4.1); 
assert_checkalmostequal(gsort(abs(d1)), gsort(abs(d0($-2:-1:$-k-1))), 1.e-10);

d1 = eigs(A, eye(n,n), k, 'LM');
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10);

opts.cholB = 1;
d1 = eigs(A, eye(n,n), k, 'LM', opts);
assert_checkalmostequal(abs(d1), abs(d0(k:-1:1)), 1.e-10); 

opts.cholB = 1;
d1 = eigs(A, eye(n,n), k, 4.1, opts);
assert_checkalmostequal(abs(eigs(A, [],k, 4.1)), abs(d1), 1.e-10);  

[d1, v1] = eigs(A, [], k, 'LM');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LR');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SR');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'LI');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

[d1, v1] = eigs(A, [], k, 'SI');
assert_checkalmostequal(A*v1, v1*d1,sqrt(%eps), 1.e-10);

