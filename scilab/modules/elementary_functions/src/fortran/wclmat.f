c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
C/MEMBR ADD NAME=WCLMAT,SSI=0
c
      subroutine wclmat(ia, n, ar, ai, br, bi, ib, w, c, ndng)
c
c%purpose
c      computes a complex matrix polynomial representated in a
c      chebychev base by the clenshaw method.
c
c%calling sequence
c
c     subroutine wclmat(ia, n, ar, ai, br, bi, ib, w, c, ndng)
c
c     integer ia,n,ib,ndng
c     double precision ar,ai,br,bi,w,c
c     dimension ar(ia,n),ai(ia,n),br(ib,n),bi(ib,n),c(*),w(*)
c
c      ia: the leading dimension of array a.
c      n: the order of the matrices a,b.
c      ar,ai : the  array that contains the n*n matrix a
c      br,bi : the  array that contains the n*n matrix
c         pol(a).
c      ib:the leading dimension of array b.
c      w : work-space array of size 4*n
c      c:  vectors which contains the coefficients
c      of the polynome.
c      ndng: the polynomial order.
c
c%auxiliary routines
c     wmmul (blas.extension)
c%
c
      integer ia,n,ib,ndng
      double precision ar,ai,br,bi,w,c
      dimension ar(ia,n),ai(ia,n),br(ib,n),bi(ib,n),c(*),w(*)
c internal variables
c
      integer k1r,k1i,k2r,k2i,i1,i,im1,j,ndng1,ndng2
      double precision two,zero,rc,wd,w1,half
      data zero, two, half /0.0d+0,2.0d+0,0.50d+0/
c
      k1r=1
      k1i=k1r+n
      k2r=k1i+n
      k2i=k2r+n
      n4=4*n
      ndng1 = ndng + 2
      ndng2 = ndng - 1
      rc = c(ndng1-1)
      wd = c(1)
      do 60 j=1,n
         do 10 i=1,n4
            w(i) = zero
   10    continue
         do 30 i1=1,ndng
            im1 = ndng1 - i1
            call wmmul(ar,ai,ia,w(k1r),w(k1i),n,br(1,j),bi(1,j),
     *                ib,n,n,1)
            do 20 i=1,n
               w1 = two*br(i,j) - w(k2r-1+i)
               w(k2r-1+i) = w(k1r-1+i)
               w(k1r-1+i) = w1
               w1 = two*bi(i,j) - w(k2i-1+i)
               w(k2i-1+i) = w(k1i-1+i)
               w(k1i-1+i) = w1
   20       continue
            w(j) = w(j) + c(im1)
   30    continue
      call wmmul(ar,ai,ia,w(k1r),w(k1i),n,br(1,j),bi(1,j),ib,n,n,1)
         do 40 i=1,n
            w(k1r-1+i) = two*br(i,j) - w(k2r-1+i)
            w(k1i-1+i) = two*bi(i,j) - w(k2i-1+i)
   40    continue
         w(j) = w(j) + wd
         do 50 i=1,n
            br(i,j) = (w(k1r-1+i)-w(k2r-1+i))*half
            bi(i,j) = (w(k1i-1+i)-w(k2i-1+i))*half
   50    continue
         br(j,j) = br(j,j) + half*wd
   60 continue
      return
      end
