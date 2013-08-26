c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1985-2008 - INRIA - Serge STEER
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
C/MEMBR ADD NAME=IMPTRA,SSI=0
c     Copyright INRIA
      subroutine imptra(pm1,d1,ld1,pm2,d2,m,n)
c!but
c     cette subroutine transpose une matrice dont les coefficients sont
c     des polynomes entiers ou des strings.
c!liste d'appel
c
c    subroutine imptra(pm1,d1,ld1,pm2,d2,m,n)
c    integer pm1(*),pm2(*)
c    integer d1(*),d2(n,m),m,n,iw(n)
c
c     pm1 : tableau entier contenant les coefficients des
c           polynomes.
c     d1 : tableau entier de taille ld1*n+1,  si k=i+(j-1)*ld1 alors
c          d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c          du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c          d1(k+1)-d1(k) -1
c     ld1 : entier definissant le rangement dans d1
c
c     pm2,d2 : definitions similaires a celles de pm1,d1, ld2
c                 est suppose  egal a n
c     m : nombre de lignes de la matrice pm1
c     n : nombre de colonne de matrice pm1
c!
      integer  pm1(*),pm2(*)
      integer d1(*),d2(*),m,n
      d2(1)=1
      i2=1
      do 20 i=1,m
      i1=i
      do 10 j=1,n
      l1=d1(i1)
      n1=d1(i1+1)-l1
      l2=d2(i2)
      call icopy(n1,pm1(l1),1,pm2(l2),1)
      i1=i1+ld1
      i2=i2+1
      d2(i2)=l2+n1
   10 continue
   20 continue
c
      return
      end
