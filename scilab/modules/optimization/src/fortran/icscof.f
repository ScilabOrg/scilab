c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
C/MEMBR ADD NAME=ICSCOF,SSI=0
c
      subroutine icscof(ico,ntob,nex,nob,yob,ob,cof)
c     ce programme est appele par les macros icsua (ico=1) et icsuq
c     (ico=2) de icse.bas pour le calcul initial des coefficients
c     de ponderation du cout
      implicit double precision (a-h,o-z)
      dimension yob(nob,ntob),ob(nex,ntob,nob),cof(nob,ntob)
c
c     en entree:(pour ico=2)
c
c     yob      double precision (nob,ntob)
c              yob=obs*ytob,avec obs(nob,ny) matrice d'observation et
c              ytob(ny,ntob) valeurs calculees de l'etat aux instants
c              de mesure
c
c     ob       double precision (nex,ntob,nob)
c              mesures
c
c     en sortie:
c
c     cof      double precision (nob,ntob)
c              coefficients de ponderation du cout
c
      do 5 i=1,nob
      do 5 j=1,ntob
5     cof(i,j)=0.0d+0
c     si ico=1 (macro icsua:ponderation "arithmetique" du cout)
c     les coefficients de ponderation du cout cof(nob,ntob)
c     sont:cof(i,j)=nex/(|ob(1,j,i)|+..+|ob(nex,j,i)|)
      if (ico.eq.1) then
      do 10 i=1,nob
      do 10 j=1,ntob
      do 10 k=1,nex
10    cof(i,j)=cof(i,j)+abs(ob(k,j,i))
      do 15 i=1,nob
      do 15 j=1,ntob
15    cof(i,j)=dble(nex)/cof(i,j)
c     si ico=2 (macro icsuq:ponderation "quadratique" du cout)
c     les coefficients de ponderation du cout cof(nob,ntob) sont:
c cof(i,j)=1/2*[(yob(i,j)-ob(1,j,i))**2+..+(yob(i,j)-ob(nex,j,i))**2]
      else
      do 20 i=1,nob
      do 20 j=1,ntob
      do 20 k=1,nex
20    cof(i,j)=cof(i,j)+(yob(i,j)-ob(k,j,i))**2
      do 25 i=1,nob
      do 25 j=1,ntob
25    cof(i,j)=0.50d+0/cof(i,j)
      endif
      return
      end
