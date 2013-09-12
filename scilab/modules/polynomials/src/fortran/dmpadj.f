c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA - Serge STEER
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine dmpadj(pm1,d1,m,n)
c!but
c     cette subroutine determine les degres effectifs (en eliminant les
c     monomes nuls de plus haut degre ) des polynomes d'une  matrice de
c     polynome.
c!liste d'appel
c
c     subroutine dmpadj(pm1,d1,m,n)
c     pm1 : tableau reel contenant les coefficients des polynomes,
c        le coefficient de degre k du polynome pm1(i,j) est range
c        dans pm1( d1(i + (j-1)*l1 + k) )
c        pm1 doit etre de taille au moins d1(l1*n+1)-d1(1)
c     d1 : tableau entier de taille l1*n+1,  si k=i+(j-1)*l1 alors
c        d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c        du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c        d1(k+1)-d1(k) -1
c     Pm1 et d1 sont modifies par l'execution
c
c     m : nombre de ligne des matrices pm
c     n : nombre de colonnes des matrices pm
      double precision pm1(*)
      integer d1(*),m,n,dj
c
      integer n1
c
      kk=1
      dj=1
c     boucle sur les polynomes
      do 40 j=1,m*n
         k1=dj-1
         n1=d1(j+1)-dj+1
 10      n1=n1-1
         if(pm1(k1+n1).eq.0.0d+0.and.n1.gt.1) goto 10
         if(kk.ne.k1+1.and.n1.gt.0) then
            do 11 i=1,n1
               pm1(kk-1+i)=pm1(k1+i)
 11         continue
         endif
         kk=kk+n1
         dj=d1(j+1)
         d1(j+1)=kk
 40   continue
c
      return
      end
