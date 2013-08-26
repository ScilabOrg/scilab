c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2005 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine zbesjg (x1r,x1i, alpha, kode, n, yr,yi, nz,wr,wi,ierr)
c     Author Serge Steer, 2005
c     extends cbesj for the case where alpha is negative
      double precision alpha
      double precision x1r, x1i, yr(n), yi(n), wr(2*n),wi(2*n)
      integer kode,n,nz,ierr
c
      double precision a,b,pi,inf,eps,xr, xi,a1
      integer ier1,ier2
      double precision dlamch

      data pi /3.14159265358979324D0/
      inf=dlamch('o')*2.0d0   
      eps=dlamch('p')

      xr=x1r
      xi=x1i
      ier2=0
      if (xr.ne.xr.or.xi.ne.xi.or.alpha.ne.alpha) then
c     .  NaN case
         call dset(n,inf-inf,yr,1)
         call dset(n,inf-inf,yi,1)
         ier1=4
      elseif (alpha .ge. 0.0d0) then
c     .  alpha >= 0 the simple case CHECKED
         call zbesj(xr,xi,alpha,kode,n,yr,yi,nz,ierr)
         if (ierr.eq.2) then
            call dset(n,inf,yr,1)
            call dset(n,inf,yi,1)
         elseif(ierr.ge.4) then
            call dset(n,inf-inf,yr,1)
            call dset(n,inf-inf,yi,1)
         endif
      else if (alpha .eq. dint(alpha)) then
c     .  alpha <0 and integer,  CHECKED
c     .  transform to positive value of alpha         
         if(alpha-1+n.ge.0) then 
c     .     0 is between alpha and alpha+n
            a1=0.0d0
            nn=min(n,int(-alpha))
         else
            a1=-(alpha-1+n)
            nn=n
         endif
         call zbesj(xr,xi,a1,kode,n,wr,wi,nz,ierr)
         if (ierr.eq.2) then
            call dset(n,inf,yr,1)
            call dset(n,inf,yi,1)
         else
            if(n.gt.nn) then
c     .        0 is between alpha and alpha+n
               call dcopy(n-nn,wr,1,yr(nn+1),1)
               call dcopy(n-nn,wi,1,yi(nn+1),1)
               call dcopy(nn,wr(2),-1,yr,1)
               call dcopy(nn,wi(2),-1,yi,1)
            else
c     .        alpha and alpha+n are negative
               call dcopy(nn,wr,-1,yr,1)
               call dcopy(nn,wi,-1,yi,1)
            endif
         endif
c     .  apply parity
         i0=mod(int(abs(alpha))+1,2)
         call dscal((nn-i0+1)/2,-1.0d0,yr(1+i0),2)
         call dscal((nn-i0+1)/2,-1.0d0,yi(1+i0),2)
      else if (xr .eq. 0.0d0.and.xi .eq.0.0d0) then
         call dset(n,-inf,yr,1)
         call dset(n,0.0d0,yi,1)
         ierr=2
      else
c     .  alpha is negative non integer CHECKED
c     .  transform to positive value of alpha         
         if(alpha-1.0d0+n.ge.0.0d0) then 
c     .     0 is between alpha and alpha+n
            nn=int(-alpha)+1
         else
            nn=n
         endif
         a1=-(alpha-1.0d0+nn)
         call zbesj(xr,xi,a1,kode,nn,yr,yi,nz1,ierr)
         call zbesy(xr,xi,a1,kode,nn,wr,wi,nz2,wr(n+1),wi(n+1),ier)
         ierr=max(ierr,ier)

         if (ierr.eq.0) then
            a=cos(a1*pi)
            b=-sin(a1*pi)
c     .     to avoid numerical errors if a is near 0 (or b near 0)  
c     .     and y is big (or w is big)
            if(abs(abs(a)-1.0d0).lt.eps)  b=0.0d0
            if(abs(abs(b)-1.0d0).lt.eps)  a=0.0d0
            call dscal(nn,b,wr,1)
            call dscal(nn,b,wi,1)
            call daxpy(nn,a,yr,1,wr,1)
            call daxpy(nn,a,yi,1,wi,1)
         elseif (ierr.eq.2) then
            call dset(nn,inf,wr,1)
            call dset(nn,inf,wi,1)
         elseif (ierr.eq.4) then
            call dset(nn,inf-inf,wr,1)
            call dset(nn,inf-inf,wi,1)
         endif
c     .  change sign to take into account that cos((a1+k)*pi) and
C     .  sin((a1+k)*pi) change sign with k
         if (nn.ge.2) then
            call dscal(nn/2,-1.0d0,wr(2),2)
            call dscal(nn/2,-1.0d0,wi(2),2)
         endif
c     .  store the result in the correct order
         call dcopy(nn,wr,-1,yr,1)
         call dcopy(nn,wi,-1,yi,1)
c     .  compute for positive value of alpha+k is any
         if (n.gt.nn) then
c     .     this code is taken from the alpha>0 case above
            a1=1.0d0-a1
            call zbesj(xr,xi,a1,kode,n-nn,yr(nn+1),yi(nn+1),nz,ier)
            if (ier.eq.2) then
               call dset(n-nn,inf,yr(nn+1),1)
               call dset(n-nn,inf,yi(nn+1),1)
            elseif(ier.ge.4) then
               call dset(n-nn,inf-inf,yr(nn+1),1)
               call dset(n-nn,inf-inf,yi(nn+1),1)
            endif
            ierr=max(ierr,ier)
         endif
  
      endif
      end

      subroutine zbesjv (xr,xi,nx,alpha,na, kode,yr,yi,wr,wi,ierr)
c     Author Serge Steer, Copyright INRIA, 2005
c     compute besseli function for x and alpha given by vectors
c     w : working array of size 3*na (used only if nz>0 and alpha contains negative
C         values
      double precision xr(nx),xi(nx),alpha(na),yr(*),yi(*),wr(*),wi(*)
      double precision dlamch,eps
      integer kode,nx,na,ier
      ierr=0
      eps=dlamch('p')
      if (na.lt.0) then
c     .  element wise case x and alpha are supposed to have the same size
         do i=1,nx
            call  zbesjg (xr(i), xi(i), alpha(i), kode, 1, yr(i),yi(i),
     $            nz,wr,wi,ier)
         enddo
      elseif (na.eq.1) then
         do i=1,nx
            call  zbesjg (xr(i), xi(i), alpha(1), kode, 1, yr(i),yi(i),
     $            nz,wr,wi,ier)
            ierr=max(ierr,ier)
         enddo
      else
c     .  compute besseli(x(i),y(j)), i=1,nx,j=1,na
         j0=1
 05      n=0
 10      n=n+1
         j=j0+n
         if (j.le.na.and.abs((1+alpha(j-1))-alpha(j)).le.eps) then
            goto 10
         endif

          do i=1,nx
            call zbesjg (xr(i), xi(i), alpha(j0) ,kode, n, wr, wi, nz,
     $           wr(na+1),wi(na+1),ier)
            ierr=max(ierr,ier)
            call dcopy(n,wr,1,yr(i+(j0-1)*nx),nx)
            call dcopy(n,wi,1,yi(i+(j0-1)*nx),nx)
         enddo
         j0=j
         if (j0.le.na) goto 05
      endif
      end


