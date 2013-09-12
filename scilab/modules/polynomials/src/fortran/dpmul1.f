c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA - Serge STEER
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
C/MEMBR ADD NAME=DPMUL1,SSI=0
c     Copyright INRIA
      subroutine dpmul1(p1,d1,p2,d2,p3)
c!but
c  ce sous programme effectue le produit polynomial:
c
c                p3(x) = p1(x) * p2(x)
c
c!liste d'appel
c      subroutine dpmul1(p1,d1,p2,d2,p3)
c     double precision p1(d1+1),p2(d2+1),p3(d1+d2+1)
c     integer d1,d2,d3
c
c     p1 : contient les coefficient du premier polynome ranges
c          suivant les puissances croissantes
c     p2 : contient les coefficients du second polynome ranges
c          suivant les puissances croissantes
c     p3 :contient les coefficient du resultats.
c         p3 peut designer la meme adresse que p1 ou p2
c     d1,d2 : degre respectifs des  polynomesp1 et p2
c!sous programmes appeles
c     ddot (blas)
c     min (fortran)
c!
c &&var
      double precision p1(*),p2(*),p3(*),ddot
      integer d1,d2,d3
      integer k,l1,l2,l3,l,m3
c &&ker
      l=1
      l1=d1+1
      l2=d2+1
      d3=d1+d2
      l3=d3+1
c
      m3=min(l1,l2)
      do 10 k=1,m3
      p3(l3)=ddot(l,p1(l1),1,p2(l2),-1)
      l=l+1
      l3=l3-1
      l1=l1-1
      l2=l2-1
   10 continue
      l=l-1
c
      if(l1.eq.0) goto 30
      m3=l1
      do 20 k=1,m3
      p3(l3)=ddot(l,p1(l1),1,p2,-1)
      l1=l1-1
      l3=l3-1
   20 continue
      goto 40
   30 if(l2.eq.0) goto 40
      m3=l2
      do 31 k=1,m3
      p3(l3)=ddot(l,p1,1,p2(l2),-1)
      l2=l2-1
      l3=l3-1
   31 continue
c
   40 if(l3.eq.0) return
      m3=l3
      do 41 k=1,m3
      l=l-1
      p3(l3)=ddot(l,p1,1,p2,-1)
      l3=l3-1
   41 continue
      return
      end
