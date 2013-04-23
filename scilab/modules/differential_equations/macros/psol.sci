//[r,ier] = psol(neq,R,iR,b)
function [r, ier] = psol(neq, R, iR, b)

    //Compute the LU factorization of R.
    sp = sparse(iR, R);
    [h, rk] = lufact(sp);

    //Solve the system LU*X = b, overwriting B with X.
    r = lusolve(h, b);
    ludel(h);

    ier = 0;
endfunction
