c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      subroutine rootgp(ngp,gpp,nbeta,beta,ierr,w)
c
c
c     Entree : - gpp. est le tableau contenant les coeff du polynome
c              gpp(z) et dont le degre est ngp.
c              - ngp. est le degre de gp(z).
c              - w tableau de travail de taille 3*ngp+1
c     Sortie : - beta. est le tableau contenant les racines du
c              polynome gpp(z) reelles comprises entre -2 et 2.
c              - nbeta. est le nombre de ces racines.
c
c!
      implicit double precision (a-h,o-z)
      dimension gpp(ngp+1),beta(*),w(*)
      logical fail
      integer ierr
cDEC$ ATTRIBUTES DLLIMPORT:: /arl2c/
      common /arl2c/ info,i1
c
c     decoupage du tableau de travail
c
      kpol=1
      kzr=kpol+ngp+1
      kzi=kzr+ngp
      kfree=kzi+ngp
c
      call dcopy(ngp+1,gpp,-1,w(kpol),1)
      call rpoly(w(kpol),ngp,w(kzr),w(kzi),fail)
      nbeta=0
      do 110 j=0,ngp-1
         if (w(kzi+j).eq.0.0d+0.and.abs(w(kzr+j)).le.2.0d+0) then
            nbeta=nbeta+1
            beta(nbeta)=w(kzr+j)
         endif
 110  continue
      if (nbeta.eq.0) then
c         if(info.ge.2) then
c         print*,' Problem : Cannot find a possible value for Beta'
c         print*,' Stopping execution immediately'
c         endif
         ierr=4
         return
      endif
      return
      end
