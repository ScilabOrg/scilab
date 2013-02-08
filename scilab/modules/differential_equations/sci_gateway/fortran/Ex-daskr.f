c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
c     Examples for daskr 
c     ----------------------
c
c
      subroutine pjac1(res, ires, neq, t, y, ydot, rewt, savr,
     *                   wk, h, cj, wp, iwp, ier, rpar, ipar)
      double precision res(*), t, y(*), ydot(*), rewt(*), savr(*),
     *                  wk(*), h, cj, wp(*), rpar(*)
      integer ires, neq, iwp(*), ier, ipar(*)
      ier = -1
c     Tell DMATD to compute J by finite differences and define uround
      iwp(4) = 2
      uround = D1MACH (4)
c     Compute the preconditioner P = dG/dy + cj*dG/dydot, LU-decompose it
c and stock it in wp and iwp
      call DMATD (neq, t, y, yprime, savr, cj, h, ier, rewt, wk, wp,
     *                iwp, res, ires, uround, jacd, rpar, ipar)
      ier = 0
      return
      end

      subroutine psol1(neq, t, y, ydot, savr, wk, cj, wght,
     *                   wp, iwp, b, eplin, ier, rpar, ipar)
      double precision t, y(*), ydot(*), savr(*), wk(*), cj, wght(*),
     *                  wp(*), b(*), eplin, rpar(*)
      integer neq, iwp(*), ier, ipar(*)
      ier = -1
      lplu = 2*neq+1
      lju  = 3*neq+2
      ljlu = 7*neq+2
      call lusol (neq, b, wk, wp, iwp, iwp)
      do 1 i=1,neq
         b(i) = wk(i)
1     continue
c      b = P\b
      ier = 0
      return
      end

      subroutine lusol(n, y, x, alu, jlu, ju)
         real*8 x(n), y(n), alu(*)
         integer n, jlu(*), ju(*)
c-----------------------------------------------------------------------
c
c This routine solves the system (LU) x = y, 
c given an LU decomposition of a matrix stored in (alu, jlu, ju) 
c modified sparse row format 
c
c-----------------------------------------------------------------------
c on entry:
c n   = dimension of system 
c y   = the right-hand-side vector
c alu, jlu, ju 
c     = the LU matrix as provided from the ILU routines. 
c
c on return
c x   = solution of LU x = y.     
c-----------------------------------------------------------------------
c 
c Note: routine is in place: call lusol (n, x, x, alu, jlu, ju) 
c       will solve the system with rhs x and overwrite the result on x . 
c
c-----------------------------------------------------------------------
c local variables
c
      integer i,k
c
c forward solve
c
      do 40 i = 1, n
         x(i) = y(i)
         do 41 k=jlu(i),ju(i)-1
            x(i) = x(i) - alu(k)* x(jlu(k))
 41      continue
 40   continue
c
c     backward solve.
c
      do 90 i = n, 1, -1
         do 91 k=ju(i),jlu(i+1)-1
            x(i) = x(i) - alu(k)*x(jlu(k))
 91     continue
            x(i) = alu(i)*x(i)
 90   continue
c
      return
c----------------end of lusol ------------------------------------------
      end
