
nx    = 10;

nev   = 3;
ncv   = 6;
bmat  = 'I';
which = 'LM';

// Local Arrays

iparam  = zeros(11, 1);
ipntr   = zeros(14, 1);
_select = zeros(ncv, 1);
d       = zeros(nev + 1, 1) + 0 * %i;
z       = zeros(nx, nev) + 0* %i;
resid   = zeros(nx, 1) + 0 * %i; 
v       = zeros(nx, ncv) + 0 * %i;
workd   = zeros(3 * nx, 1) + 0 * %i; 
workev  = zeros(2 * ncv, 1) + 0 * %i;
rwork   = zeros(ncv, 1);
workl   = zeros(3 * ncv * ncv + 5 *ncv, 1) + 0 * %i;

// Build the complex test matrix
A            = diag(10 * ones(nx,1) + %i * ones(nx,1));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6 * ones(nx - 1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(-6 * ones(nx - 1,1));

tol    = 0;
ido    = 0;

ishfts = 1;
maxitr = 300;
mode1  = 1;

iparam(1) = ishfts;
iparam(3) = maxitr;
iparam(7) = mode1;

sigma = complex(0); 
info_znaupd = 0;
// M A I N   L O O P (Reverse communication)
while(ido <> 99)
  // Repeatedly call the routine ZNAUPD and take actions indicated by parameter IDO until
  // either convergence is indicated or maxitr has been exceeded.

  [ido, resid, v, iparam, ipntr, workd, workl, info_znaupd] = znaupd(ido, bmat, nx, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, rwork, info_znaupd);
  
  if(info_znaupd < 0)
    printf('\nError with znaupd, info = %d\n', info_znaupd);
    printf('Check the documentation of znaupd\n\n');
  end
  
  if(ido == -1 | ido == 1)
    // Perform matrix vector multiplication 
    workd(ipntr(2):ipntr(2) + nx - 1) = A * workd(ipntr(1):ipntr(1) + nx - 1);
  end
end
  
// Post-Process using ZNEUPD.

rvec    = 1;
howmany = 'A';
info_zneupd = 0;

[d, z, resid, iparam, ipntr, workd, workl, rwork, info_zneupd] = zneupd(rvec, howmany, _select, d, z, sigma, workev, bmat, nx, which, nev, tol, resid, ncv, v, ...
                                                                    iparam, ipntr, workd, workl, rwork, info_zneupd);

assert_checkequal(nx, 10);
assert_checkequal(nev, 3);
assert_checkequal(ncv, 6);
assert_checkequal(which, "LM");
// This variable can vary. Do not test it.
// assert_checkequal(iparam(3), 40);
// This variable can vary. Do not test it.
// assert_checkequal(iparam(9),90);
assert_checkequal(tol, 0);
