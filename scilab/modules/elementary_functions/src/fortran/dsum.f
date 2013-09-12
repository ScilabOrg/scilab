c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1986 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
C/MEMBR ADD NAME=DSUM,SSI=0
c
      double precision function dsum(n,dx,incx)
c
c!but
c
c     cette fonction donne la somme des n composantes d'un vecteur dx.
c
c!liste d'appel
c
c     double precision function dsum(n,dx,incx)
c
c     n: entier, taille du vecteur dx.
c
c     dx: double precision, vecteur dont on veut la somme
c
c     incx: increment entre deux composantes consecutives de dx.
c
c!auteur
c
c     serge Steer ,inria 86
c!
c
      double precision dx(*),dtemp
      integer i,incx,n,nincx
c
      dsum = 0.0d+0
      dtemp = 0.0d+0
      if(n.le.0)return
      if(incx.eq.1)go to 20
c
c code for increment not equal to 1
c
      nincx = n*incx
      do 10 i = 1,nincx,incx
        dtemp = dtemp + dx(i)
   10 continue
      dsum = dtemp
      return
c
c code for increment equal to 1
c
   20 continue
      do 30 i = 1,n
        dtemp = dtemp + dx(i)
   30 continue
      dsum = dtemp
c
      end
