// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) 2005 - IRISA - Sage Group
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [x, flag, resNorm, iter, resVec] = pcg(%A, %b, tol, maxIter, %M, %M2, x)
// PCG solves the symmetric positive definite linear system %Ax=b 
// using the Preconditionned Conjugate Gradient.
//
// input   %A        REAL symmetric positive definite matrix or a function 
//                  y=Ax(x) which computes  y=%A*x for a given x
//         b        REAL right hand side vector
//         tol      REAL error tolerance (default: 1e-8)
//         maxIter  INTEGER maximum number of iterations (default: 50)
//         %M        REAL preconditioner matrix (default: none)
//         %M2       REAL preconditioner matrix (default: none)
//         x        REAL initial guess vector
//
// output  x        REAL solution vector
//         flag     INTEGER: 0 = solution found to tolerance
//                           1 = no convergence given maxIter
//         resNorm  REAL final relative norm of the residual
//         iter     INTEGER number of iterations performed
//         resVec   REAL residual vector

//     Details of this algorithm are described in 
//
//     "Templates for the Solution of Linear Systems: Building Blocks 
//     for Iterative Methods", 
//     Barrett, Berry, Chan, Demmel, Donato, Dongarra, Eijkhout,
//     Pozo, Romine, and Van der Vorst, SIAM Publications, 1993
//     (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps).
//
//     "Iterative Methods for Sparse Linear Systems, Second Edition"
//     Saad, SIAM Publications, 2003
//     (ftp ftp.cs.umn.edu; cd dept/users/saad/PS; get all_ps.zip).

// Sage Group (IRISA, 2004)
// -----------------------
// Parsing input arguments
// -----------------------
  [lhs,rhs] = argn(0);
  if (rhs < 2),
	error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"pcg",2,7));
  end
  if (rhs > 7),
	error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"pcg",2,7));
  end
  if exists('tol','local')==0 then tol=1e-8,end
  if exists('maxIter','local') == 0 then maxIter=min(size(b,1),50);end
  if exists('%M','local')       == 0 then precondType = 0;%M=[];     end
  if exists('%M2','local')      == 0 then precondBis = 0;%M2=[];     end
  if exists('x','local')       == 0 then x=zeros(b);               end
  
  
  // Parsing the matrix %A
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
    %A=%A(1);
    matrixType = 0;
  else
	error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"pcg",1));
  end

  // If %A is a matrix (dense or sparse)
  if (matrixType == 1),
    if (size(%A,1) ~= size(%A,2)),
	  error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"pcg",1));
    end
  end

  // Parsing right hand side %b
  if (size(%b,2) ~= 1),
	error(msprintf(gettext("%s: Wrong type for input argument #%d: Column vector expected.\n"),"pcg",2));
  end
  if (matrixType ==1),
    if (size(%b,1) ~= size(%A,1)),
	  error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"pcg",2,1));
    end 
  end

  // Parsing of the error tolerance tol
  if or(size(tol) ~= [1 1]) then
	error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar expected.\n"),"pcg",3));

  end

  // Parsing of the maximum number of iterations max_it
  if or(size(maxIter) ~= [1 1]) then
	error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar expected.\n"),"pcg",4));
  end 

  // Parsing the preconditioner %M
  select type(%M)
  case 1 then
    precondType = bool2s(size(%M,'*')>=1);
  case 5 then
    precondType = 1;
  case 13 then
    Margs=list()
    precondType = 2;
  case 15 then
    Margs=list(%M(2:$))
    %M=%M(1);
    precondType = 2;
  else
	error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"pcg",5));
  end 
  if (precondType == 1),
    if (size(%M,1) ~= size(%M,2)),
	  error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"pcg",5));
    end 
    if ( size(%M,1) ~= size(%b,1) ),
	  error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"pcg",5,2));
    end
  end

  // Parsing the preconditioner %M
  select type(%M2)
  case 1 then
    precondBis =bool2s(size(%M2,'*')>=1);
  case 5 then
    precondBis = 1;
  case 13 then
    M2args=list()
    precondBis = 2;
  case 15 then
    M2args=list(%M2(2:$))
    %M2=%M2(1);
    precondType = 2;
  else
	error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"pcg",6));
   end

  if (precondBis == 1),
    if (size(%M2,1) ~= size(%M2,2)),
	  error(msprintf(gettext("%s: Wrong type for input argument #%d: Square matrix expected.\n"),"pcg",6));
    end 
    if ( size(%M2,1) ~= size(%b,1) ),
	  error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"pcg",6,2));
    end
  end

  // Parsing the initial vector x
  if (size(x,2) ~= 1),
	error(msprintf(gettext("%s: Wrong value for input argument #%d: Column vector expected.\n"),"pcg",7));
  end
  if (size(x,1) ~= size(%b,1)),
	error(msprintf(gettext("%s: Wrong size for input argument #%d: Same size as input argument #%d expected.\n"),"pcg",7,2));
  end 
  
  // input arguments are parsed !
  
  // ------------
  // Computations
  // ------------

  // initialization

  flag = 0; 
  iter = 0;
  bnrm2 = norm(%b);
  if  (bnrm2 == 0) then
    x = zeros(%b);
    resNorm = 0;
    resVec = resNorm;
  end

  // r = %b - %A*x;
  if (matrixType ==1),
    r = %b - %A*x;
  else
    r = %b - %A(x,Aargs(:));
  end
  resNorm = norm(r) / bnrm2;
  resVec = resNorm;
  if (resNorm < tol) then return; end
  // begin iteration
  for i = 1:maxIter-1 
    // z  = %M \ r;
    if (precondType == 1),
      z = %M \ r;
    elseif (precondType == 2),
      z = %M(r,Margs(:));
    else
      z = r;
    end
    // z  = %M2 \ r;
    if (precondBis == 1) then
      z = %M2 \ r;
    elseif (precondBis == 2) then
      z = %M2(r,M2args(:));
    else
      z = r;
    end
    rho = (r'*z);
    if (i > 1) then
      bet = rho / rho_1;
      p = z + bet*p;
    else
      p = z;
    end
    
    // q = %A*p;
    if (matrixType ==1),
      q = %A*p;
    else
      q = %A(p);
    end
    alp = rho / (p'*q );
    x = x + alp*p;
    r = r - alp*q;
    resNorm = norm(r) / bnrm2;
    resVec = [resVec;resNorm];
    if (resNorm <= tol) then iter=i;break;end 
    rho_1 = rho;
    if (i == maxIter ) then iter=i;end
  end
  // test for convergence
  if (resNorm > tol) then
    flag = 1; 
    if (lhs < 2) then warning(msprintf(gettext("%s: Convergence error.\n"),"pcg"));end
  end
  
endfunction
