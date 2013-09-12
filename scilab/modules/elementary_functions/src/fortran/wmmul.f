c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine wmmul(Ar,Ai,na,Br,Bi,nb,Cr,Ci,nc,l,m,n)
*
*     PURPOSE
*        computes the matrix product C = A * B where the
*        matrices are complex with the scilab storage
*            C   =   A   *   B
*          (l,n)   (l,m) * (m,n)
*       
*     PARAMETERS
*        input 
*        -----
*        Ar, Ai : real and imaginary part of the matrix A
*                 (double) arrays (l, m) with leading dim na
*                 
*        Br, Bi : real and imaginary part of the matrix B
*                 (double) arrays (m, n) with leading dim nb
*    
*        na, nb, nc, l, m, n : integers
*
*        output 
*        ------
*        Cr, Ci : real and imaginary part of the matrix C
*                 (double) arrays (l, n) with leading dim nc
*
*     METHOD
*        Cr = Ar * Br - Ai * Bi
*        Ci = Ar * Bi + Ai * Br
*
*     NOTE
*        modification of the old wmmul to use blas calls
*
      implicit none

      integer na, nb, nc, l, m, n
      double precision Ar(na,m), Ai(na,m), Br(nb,n), Bi(nb,n), 
     $                 Cr(nc,n), Ci(nc,n)

*     Cr <-  1*Ar*Br + 0*Cr
      call dgemm('n','n', l, n, m, 1.d0, Ar, na, Br, nb, 0.d0, Cr, nc)
*     Cr <- -1*Ai*Bi + 1*Cr
      call dgemm('n','n', l, n, m,-1.d0, Ai, na, Bi, nb, 1.d0, Cr, nc)
*     Ci <-  1*Ar*Bi + 0*Ci
      call dgemm('n','n', l, n, m, 1.d0, Ar, na, Bi, nb, 0.d0, Ci, nc)
*     Ci <-  1*Ai*Br + 1*Ci
      call dgemm('n','n', l, n, m, 1.d0, Ai, na, Br, nb, 1.d0, Ci, nc)

      end


