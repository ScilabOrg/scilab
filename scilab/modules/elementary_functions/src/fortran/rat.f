c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine rat(x,eps,n,d,fail)
c!but
c     ce sous programme approximme un reel x par un rationnel n/d
c!liste d'appel
c
c       subroutine rat(x,eps,n,d,fail)
c     double precision x,eps
c     integer n,d,fail
c
c     x: reel contenant le nombre a approximer
c     eps: precision requise : (abs(x-n/d))<eps
c     n,d:entiers contenant respectivement le numerateur et le
c         denominateur du resultat
c     fail:indicateur d'erreur
c           fail=0 : ok
c           fail.ne.0 precision requise trop grande ou nombre trop
c           grand ou trop petit pour etre code sous cette forme
c!origine
c     s steer inria
c
      double precision x,eps,z,err,dz,xn,xd,ax
      integer n,d,nmax,n0,d0,n1,d1,bm,fail
c possibly the largest integer (hum ???)
      nmax=2147483647
      fail=0
      n0=0
      d0=1
      n1=1
      d1=0
      z=abs(x)
      ax=z
   10 err=abs(d1*ax-n1)
      if(err.le.d1*eps) goto 20
      if(z.gt.nmax) goto 30
      bm=int(z)
      dz=z-bm
      if(dz.eq.0.0d+0) goto 15
      z=1.0d+0/dz
   15 xn=n0+dble(bm)*n1
      xd=d0+dble(bm)*d1
      if(xn.gt.nmax.or.xd.gt.nmax) goto 30
      n0=n1
      d0=d1
      n1=int(xn)
      d1=int(xd)
      if(dz.eq.0.0d+0) goto 20
      goto 10
   20 n=n1
      d=d1
      if (x.lt.0.0d+0) n=-n
      return
   30 fail=1
      return
      end
