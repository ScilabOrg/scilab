// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// cgs --
//   CGS solves the linear system %Ax=b using the Conjugate Gradient Squared method.
//   If M is given, it is used as a preconditionning matrix.
//   If both M and M2 are given, the matrix M * M2 is used as a preconditionning
//   matrix.
//
// input   %A       REAL matrix or a function y=Ax(x) which computes y=%A*x for a given x
//         b        REAL right hand side vector
//         tol, optional      REAL error tolerance (default: 1e-8)
//         maxIter, optional  INTEGER maximum number of iterations (default: size(%b))
//         %M, optional       REAL preconditioner matrix (default: none)
//         %M2, optional      REAL preconditioner matrix (default: none)
//         x0, optional       REAL initial guess vector (default: the zero vector)
//         verbose, optional  INTEGER set to 1 to enable verbose logging (default : 0)
//
// output  x        REAL solution vector
//         flag     INTEGER: 0 = solution found to tolerance
//                           1 = no convergence given maxIter
//         resNorm  REAL final relative norm of the residual
//         iter     INTEGER number of iterations performed
//         resVec   REAL residual vector
//
// References
//
//     "CGS, A Fast Lanczos-Type Solver for Nonsymmetric Linear systems"
//     by Peter Sonneveld
//
//     http://epubs.siam.org/doi/abs/10.1137/0910004
//     http://dl.acm.org/citation.cfm?id=64888&preflayout=flat
//     http://mathworld.wolfram.com/ConjugateGradientSquaredMethod.html
//
// Notes
//     The input / output arguments of this command are the same as Matlab's cgs command.
//
function [x, flag, resNorm, iter, resVec] = cgs(%A, %b, tol, maxIter, %M, %M2, x0, verbose )
    [lhs,rhs] = argn(0);
    if (rhs < 2),
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"cgs",2,7));
    end
    if (rhs > 7),
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"cgs",2,7));
    end
    if exists("tol","local")==0 then
        tol = 1e-8
    end
    if exists("maxIter","local")==0 then
        maxIter = size(%b,1)
    end
    if exists("%M","local")==0 then
        %M=[]
    end
    if exists("%M2","local")==0 then
        %M2=[]
    end
    if exists("x0","local")==0 then
        x0=zeros(%b);
    end
    if exists("verbose","local")==0 then
        verbose=0;
    end
    if (verbose==1) then
        printf(gettext("Arguments:\n"));
        printf("  tol="+string(tol)+"\n");
        printf("  maxIter="+string(maxIter)+"\n");
        printf("  M=\n")
        disp(%M)
        printf("  M2=\n");
        disp(%M2)
        printf("  x0=\n");
        disp(x0)
        printf("  verbose="+string(verbose)+"\n");
    end
    // Compute matrixType
    select type(%A)
    case 1 then
        matrixType = 1;
    case 5 then
        matrixType = 1;
    case 13 then
        matrixType = 0;
        Aargs=list()
    case 15 then
        Aargs=list(%A(2:$))
        // Caution : modify the input argument %A !
        %A=%A(1);
        matrixType = 0;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"cgs",1));
    end
    // If %A is a matrix (dense or sparse)
    if (matrixType == 1),
        if (size(%A,1) ~= size(%A,2)),
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"cgs",1));
        end
    end
    // Check right hand side %b
    if (size(%b,2) ~= 1),
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Column vector expected.\n"),"cgs",2));
    end
    if (matrixType ==1),
        if (size(%b,1) ~= size(%A,1)),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"cgs",2,1));
        end
    end
    // Check type of the error tolerance tol
    if or(size(tol) ~= [1 1]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar expected.\n"),"cgs",3));
    end
    // Check the type of maximum number of iterations maxIter
    if or(size(maxIter) ~= [1 1]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar expected.\n"),"cgs",4));
    end
    // Compute precondType
    select type(%M)
    case 1 then
        // M is a matrix
        // precondType = 0 if the M is empty
        //             = 1 if the M is not empty
        precondType = bool2s(size(%M,"*")>=1);
    case 5 then
        precondType = 1;
    case 13 then
        Margs=list()
        precondType = 2;
    case 15 then
        Margs=list(%M(2:$))
        // Caution : modify the input argument %M !
        %M=%M(1);
        precondType = 2;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"cgs",5));
    end
    if (precondType == 1),
        if (size(%M,1) ~= size(%M,2)),
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"cgs",5));
        end
        if ( size(%M,1) ~= size(%b,1) ),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"cgs",5,2));
        end
    end
    // Compute precondBis
    select type(%M2)
    case 1 then
        // M2 is a matrix
        // precondBis = 0 if the M2 is empty
        //            = 1 if the M2 is not empty
        precondBis =bool2s(size(%M2,"*")>=1);
    case 5 then
        precondBis = 1;
    case 13 then
        M2args=list()
        precondBis = 2;
    case 15 then
        M2args=list(%M2(2:$))
        // Caution : modify the input argument %M2 !
        %M2=%M2(1);
        // Caution : modify precondType again !
        precondType = 2;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"cgs",6));
    end
    if (precondBis == 1),
        if (size(%M2,1) ~= size(%M2,2)),
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"cgs",6));
        end
        if ( size(%M2,1) ~= size(%b,1) ),
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"cgs",6,2));
        end
    end
    // Check size of the initial vector x0
    if (size(x0,2) ~= 1),
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Column vector expected.\n"),"cgs",7));
    end
    if (size(x0,1) ~= size(%b,1)),
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"cgs",7,2));
    end
    // ------------
    // Computations
    // ------------
    // initialization
    bnrm2 = norm(%b);
    if (verbose==1) then
        printf(gettext("Norm of right-hand side : %s\n"), string(bnrm2));
    end
    if  (bnrm2 == 0) then
        if (verbose==1) then
            printf(gettext("Special processing where the right-hand side is zero.\n"));
        end
        // When rhs is 0, there is a trivial solution : x=0
        x = zeros(%b);
        r = 0;
        resNorm = 0;
        resVec = resNorm;
    else
        x = x0;
        // r = %b - %A*x;
        if (matrixType ==1),
            r = %b - %A*x;
        else
            r = %b - %A(x,Aargs(:));
        end
        resNorm = norm(r) / bnrm2;
        resVec = resNorm;
    end
    if (verbose==1) then
        printf(gettext("  Type of preconditionning #1 : %d\n"),precondType);
        printf(gettext("  Type of preconditionning #2 : %d\n"),precondBis);
    end
    // begin iteration
    // Distinguish the number of iterations processed from the currentiter index
    iter = 0
    r0 = r;
    for currentiter = 1:maxIter
        if (resNorm <= tol) then
            if (verbose==1) then
                printf(gettext("  New residual = %s < tol = %s => break\n"),string(resNorm),string(tol));
            end
            break;
        end
        iter = iter + 1
        if (verbose==1) then
            printf(gettext("  Iteration #%s/%s residual : %s\n"),string(currentiter),string(maxIter),string(resNorm));
            printf("  x=\n");
            disp(x);
        end
        rho = r0'*r;
        if (currentiter > 1) then
            bet = rho / rho_old;
            u = r + bet*q;
            p = u + bet*(q+bet*p);
        else
            u = r;
            p = u;
        end
        // Solve M M2 P = p
        if %M == [] & %M2 == [] then
            P = p;
        elseif %M2 == [] then
            // Compute P so that M P = p
            if (precondType == 1) then
                P = %M \ p;
            elseif (precondType == 2) then
                P = %M(p,Margs(:));
            else
                P = p;
            end
        else
            // Compute P so that M M2 P = p
            if (precondBis == 1) then
                P = %M \ p;
                P = %M2 \ p;
            elseif (precondBis == 2) then
                P = %M(p,Margs(:));
                P = %M2(p,M2args(:));
            else
                P = p;
            end
        end
        // d = %A*P;
        if (matrixType ==1),
            d = %A*P;
        else
            d = %A(P);
        end
        alp = rho / (r0'*d);
        q = u - (alp*d);
        // Solve M M2 Q = u+q
        uq = u + q;
        if %M == [] & %M2 == [] then
            Q = uq;
        elseif %M2 == [] then
            // Compute Q so that M Q = u+q
            if (precondType == 1) then
                Q = %M \ uq;
            elseif (precondType == 2) then
                Q = %M(uq,Margs(:));
            else
                Q = uq;
            end
        else
            // Compute z so that M M2 Q = u+q
            if (precondBis == 1) then
                Q = %M \ uq;
                Q = %M2 \ uq;
            elseif (precondBis == 2) then
                Q = %M(uq,Margs(:));
                Q = %M2(uq,M2args(:));
            else
                Q = uq;
            end
        end
        x = x + alp*Q;
        if (matrixType ==1),
            Q = %A*Q;
        else
            Q = %A(Q);
        end
        r = r - alp*Q;
        resNorm = norm(r) / bnrm2;
        // Caution : transform the scalar resVec into vector resVec !
        resVec = [resVec;resNorm];
        rho_old = rho;
    end
    // test for convergence
    if (resNorm > tol) then
        if (verbose==1) then
            printf(gettext("Final residual = %s > tol =%s\n"),string(resNorm),string(tol));
            printf(gettext("Algorithm fails\n"));
        end
        flag = 1;
        if (lhs < 2) then
            warning(msprintf(gettext("%s: Convergence error.\n"),"cgs"));
        end
    else
        flag = 0;
        if (verbose==1) then
            printf(gettext("Algorithm pass\n"));
        end
    end
endfunction
