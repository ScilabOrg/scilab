c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine fmulb1(n,h,x,hx,tabaux,nmisaj,prosca,izs,rzs,dzs)
      implicit double precision (a-h,o-z)
      external prosca
c
c parametres
      double precision   un     , deux
      parameter (        un=1.0d+0, deux=2.0d+0 )
c declarations
      double precision   h(*), x(n), hx(n), tabaux(n), dzs(*)
      real rzs(*)
      integer  izs(*)
      double precision   uscalx, sscalx, nu, eta, gamma, mu, sigma
      integer      n, nmisaj, memsup, ptnu, compt, iu, is, k
c
c calcul de la longueur d'un bloc
      memsup=2*n+2
c calcul de  h0*x=x=x
      do 1000 k=1,n
      hx(k)=x(k)
1000  continue
c
      if (nmisaj.eq.0) then
      return
      else
      ptnu=1
      compt=1
      endif
c
2000  iu=ptnu+1
      is=iu+n
      do 3000 k=1,n
      tabaux(k)=h(iu+k)
3000  continue
      call prosca(n,tabaux,x,uscalx,izs,rzs,dzs)
      do 4000 k=1,n
      tabaux(k)=h(is+k)
4000  continue
      call prosca(n,tabaux,x,sscalx,izs,rzs,dzs)
      nu=h(ptnu)
      eta=h(ptnu+1)
c calcul du nouveau terme et addition dans hx
      if (compt.eq.1) then
      gamma=eta / nu
      do 5000 k=1,n
      hx(k)=gamma * hx(k)
5000  continue
      mu=sscalx / nu
      sigma=-(deux * sscalx / eta)+(uscalx / nu)
      else
      mu=sscalx / eta
      sigma=-(un + nu / eta)* mu + uscalx / eta
      endif
c
      do 6000 k=1,n
      hx(k)=hx(k) - mu * h(iu+k) - sigma * h(is+k)
6000  continue
c
      compt=compt+1
      if (compt.le.nmisaj) then
      ptnu=ptnu+memsup
      goto 2000
      else
      return
      endif
      end
