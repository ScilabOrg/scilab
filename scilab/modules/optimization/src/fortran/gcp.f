c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine gcp(n,index,indic,np,nt,y,s,z,ys,zs,diag,b,x,d,g,eps)
c
c     methode de gradient preconditionne appliquee a l'equation
c     [a]*{x}={b}. ici [a] est definie par les vecteurs ({y}(i),
c     {s}(i), {z}(i), i=1,np).
c
      implicit  double precision (a-h,o-z)
      dimension x(n),b(n),y(nt,n),s(nt,n),z(nt,n),ys(nt),zs(nt),diag(n)
      dimension       g(n),d(n)
      integer   index(nt),indic(n)
c
c     initialisation
      eps0=1.e-5
      eps1=1.e-5
      do 100 i=1,n
         if(indic(i).gt.0) go to 100
         x(i)=-b(i)/diag(i)
100   continue
c
      call calbx(n,index,indic,nt,np,y,s,ys,z,zs,x,diag,g)
      do 110 i=1,n
         if(indic(i).gt.0) go to 110
         g(i)=g(i)+b(i)
110   continue
c
c     ----------
c     iteration 1
c     ------test de convergence
      s0=0
      do 120 i=1,n
         if(indic(i).gt.0) go to 120
         s0=s0+g(i)*g(i)/diag(i)
120   continue
      if(s0.lt.1.0d-18) return
      s1=s0
c     ------recherche de la direction de descente
      do 130 i=1,n
         if(indic(i).gt.0) go to 130
         d(i)=-g(i)/diag(i)
130   continue
c
c     ------step length
      dg=0.
      do 135 i=1,n
         if(indic(i).gt.0) go to 135
         dg=dg+d(i)*g(i)
135   continue
      call calbx(n,index,indic,nt,np,y,s,ys,z,zs,d,diag,g)
      d2a=0
      do 140 i=1,n
         if(indic(i).gt.0) go to 140
         d2a=d2a+d(i)*g(i)
140   continue
c
      ro=-dg/d2a
      do 150 i=1,n
         if(indic(i).gt.0) go to 150
         x(i)=x(i)+ro*d(i)
150   continue
      call calbx(n,index,indic,nt,np,y,s,ys,z,zs,x,diag,g)
      do 170 i=1,n
         if(indic(i).gt.0) go to 170
         g(i)=g(i)+b(i)
170   continue
c
c     iteration k
      iter=0
      itmax=2*np
10    iter=iter +1
      if(iter.gt.itmax)return
c     ------test de convergence
      s2=0
      do 200 i=1,n
         if(indic(i).gt.0) go to 200
         s2=s2+g(i)*g(i)/diag(i)
200   continue
      if((s2/s0).lt.eps) return
c     ------recherche de la direction de descente
      beta=s2/s1
      do 210 i=1,n
         if(indic(i).gt.0) go to 210
         d(i)=-g(i)/diag(i)+beta*d(i)
210   continue
      s1=s2
c
c     -----step length
      dg=0.
      do 215 i=1,n
         if(indic(i).gt.0) go to 215
         dg=dg+d(i)*g(i)
215   continue
      call calbx(n,index,indic,nt,np,y,s,ys,z,zs,d,diag,g)
      d2a=0.
      do 220 i=1,n
         if(indic(i).gt.0) go to 220
         d2a=d2a+d(i)*g(i)
220   continue
c
      ro=-dg/d2a
      do 230 i=1,n
         if(indic(i).gt.0) go to 230
         x(i)=x(i)+ro*d(i)
230   continue
      call calbx(n,index,indic,nt,np,y,s,ys,z,zs,x,diag,g)
      do 240 i=1,n
         if(indic(i).gt.0) go to 240
         g(i)=g(i)+b(i)
240   continue
      go to 10
      end
