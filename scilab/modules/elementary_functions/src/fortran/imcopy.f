c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
C/MEMBR ADD NAME=IMCOPY,SSI=0
c
      subroutine imcopy(a,na,b,nb,m,n)
c!but
c     ce sous programme effectue:b=a
c     avec a matrice m lignes et n colonnes
c     imcopy utilise un code particulier si les matrices sont
c     compactes
c!liste d'appel
c
c     subroutine imcopy(a,na,b,nb,m,n)
c     integer a(na,n),b(nb,m)
c     integer na,nb,m,n
c
c     a         tableau contenant la matrice a
c     na        nombre de lignes du tableau a dans le prog appelant
c     b,nb      definition similaires a :a,na
c     m         nombre de lignes des matrices a et b
c     n         nombre de colonnes des matrices a et b
c!sous programmes utilises
c     neant
c!
      integer a(*),b(*)
      integer na,nb,m,n
      integer ia,ib,i,j,mn
c
      if(na.eq.m .and. nb.eq.m) goto 20
      ia=-na
      ib=-nb
      do 10 j=1,n
      ia=ia+na
      ib=ib+nb
      do 10 i=1,m
      b(ib+i)=a(ia+i)
   10 continue
      return
   20 continue
c code pour des matrices compactes
      mn=m*n
      do 30 i=1,mn
      b(i)=a(i)
   30 continue
      return
      end
