// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 3770 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3770
//
// <-- Short Description -->
//    "qld" returns a error with scilab 5.0.2 and not with scilab 4.1.2
// =============================================================================
function [y]=cif4(x)
    y(1)=127.d0-2.d0*x(1)^2-3.d0*x(2)^4-x(3)-4.d0*x(4)^2 -5.d0*x(5);
    y(2)=282.d0-7.d0*x(1)-3.d0*x(2)-10.d0*x(3)^2-x(4)+x(5);
    y(3)=196.d0-23.d0*x(1)-x(2)^2-6.d0*x(6)^2+8.d0*x(7);
    y(4)=-4.d0*x(1)^2-x(2)^2+3.d0*x(1)*x(2)-2.d0*x(3)^2-5.d0*x(6) +11.d0*x(7);
endfunction
// =============================================================================
function [y]=objf4(x)
    y=(x(1)-10.d0)^2+5.d0*(x(2)-12.d0)^2+x(3)^4+3.d0*(x(4) ...
    -11.d0)^2+10.d0*x(5)^6+7.d0*x(6)^2+x(7)^4-4.d0*x(6) ...
    *x(7)-10.d0*x(6)-8.d0*x(7);
endfunction
// =============================================================================
function [x,fx,ci,lambda,info,iter]=sqp(x,objf,cif,lb,ub,ni,maxiter,tol)
    n = length(x);
    x = max(x,lb);
    x = min(x,ub);
    iter = 0;
    fx = objf(x);
    g = numdiff(objf,x)';
    H = eye(n,n);
    if (ni > 0) then
        ci = cif(x)
        if (length(ci) ~= ni) then
            error("Ci pas de longueur ni")
        end
        A = numdiff(cif,x)
    else
        ci = [];
        A = [];
    end

    [n1 n2] = size(A);

    lambda = 100*ones(n1,1);
    lambda = zeros(ni,1);

    qp_iter = 1;
    alpha = 1;

    info = 0;

    t0 = norm(g -A'*lambda);   // t0 = norme du Langrangien df/dx - lambda*dc/dx
    logic_t2 = and(ci >= -tol);
    logic_t3 = and(lambda >=0);

    t4 = norm(lambda .*ci);

    if( logic_t2 & logic_t3 & max(t0,t4) < tol) then
        break
    end

    eta = 0.25;
    tau = 0.5;

    for iter = 1:maxiter
        dinf = lb-x;
        dsup = ub-x;
        [d,lambda_qp,info_qp]=qld(H,g,-A,ci,dinf,dsup,0,tol);

        if (info_qp ~=0) then
            info = 1000 + info_qp;
            break
        end

        mu = 1/(sqrt(%eps) + norm(lambda,%inf));
        cia = ci(ci < - tol );
        phi = fx + norm(cia,1)/mu;
        D_phi = g'*d - norm(cia,1)/mu;

        alpha = 1;
        while(1)
            x_new = x + alpha*d;
            obj_new = objf(x_new);
            ci_new = cif(x_new);
            cia_new = ci_new(ci_new < -tol);
            phi_new = obj_new + norm(cia_new,1)/mu;
            phi_armijo = phi + eta*alpha*D_phi;
            if (phi_new <= phi_armijo) then
                break
            else
                alpha = 0.45*alpha;
            end
        end

        lambda = lambda + alpha*(lambda_qp(1:ni) - lambda);

        g_new = numdiff(objf,x_new)';

        if(ni>0) then
            A_new = numdiff(cif,x_new);
        else
            A_new = [];
        end

        t0 = norm(g_new -A_new'*lambda);   // t0 = norme du Langrangien df/dx - lambda*dc/dx

        logic_t2 = and(ci_new >= -tol);
        logic_t3 = and(lambda >=0);

        t4 = norm(lambda .*ci);

        if( logic_t2 & logic_t3 & max(t0,t4) < tol) then
            break
        end

        y = g_new - g;

        if(~isempty(A)) then
            t = ((A_new-A)'*lambda);
            y = y - t;
        end

        delx = x_new - x;

        if (norm(delx) < tol*norm(x)) then
            info = 101;
            break
        end

        d1 = delx'*H*delx;

        t1 = 0.2*d1;
        t2 = delx'*y;

        if (t2<t1) then
            theta = 0.8*d1/(d1-t2);
        else
            theta = 1;
        end

        r = theta*y + (1-theta)*H*delx;
        d2 = delx'*r;

        if (d1==0 | d2 ==0) then
            info = 102;
            break
        end

        H = H -H*delx*delx'*H/d1 + r*r'/d2;

        x = x_new;
        fx = obj_new;
        g = g_new;
        ci = ci_new;
        A = A_new;
    end

    if(iter>=maxiter)
        info = 103;
    end

endfunction
// =============================================================================
n = 7;
ni = 4;
x = ones(n,1);
ub = %inf*ones(x);
lb = -ub;
maxit = 20;
tol = 1e-3;
// =============================================================================
[x,fx,ci,lambda_qp,info,iter]=sqp(x,objf4,cif4,lb,ub,ni,maxit,tol);
if size(x) <> [7 1] then pause,end
if size(fx) <> [1 1] then pause,end
if size(ci) <> [4 1] then pause,end
if size(lambda_qp) <> [4 1] then pause,end
if size(info) <> [1 1] then pause,end
if size(iter) <> [1 1] then pause,end
if info <> 101 then pause,end
if iter <> 7 then pause,end
// =============================================================================
