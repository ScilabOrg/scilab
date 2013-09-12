c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine fmuls1(n,h,x,hx)
      implicit double precision (a-h,o-z)
c
c ce sous-programme effectue le produit  h * x   avec:
c n (e) dimension du probleme
c h (e) dimension n(n+1)/2. tiangle superieur, coefficients par ligne
c x (e) vecteur de dimension n
c hx (s) dimension n. resultat du produit
c
c parametre
      double precision  zero
      parameter       ( zero=0.0d+0 )
c declarations
      double precision  h(*), x(n), hx(n), aux1
      integer    n, k, km1, kj, j
c
      do 3000 k=1,n
c calcul de la keme composante du produit  h* x
      aux1=zero
c h(kj) est le coefficient (k,j) de la matrice symetrique complete
      kj=k
      km1=k-1
c contribution du triangle inferieur
      if (km1.ge.1) then
      do 1000 j=1,km1
      aux1=aux1 + h(kj) * x(j)
      kj=kj+(n-j)
1000  continue
      endif
c contribution du triangle superieur
      do 2000 j=k,n
      aux1=aux1 + h(kj) * x(j)
      kj=kj+1
2000  continue
c
      hx(k)=aux1
3000  continue
c
      return
      end
