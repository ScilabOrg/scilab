c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine mzdivq(ichoix,nv,tv,nq,tq)
c!but
c     cette routine calcule, lorsque l'on connait le quotient et le reste
c        de la division par q d'un polynome, le reste et le quotient de
c        la division par q de ce    polynome multiplie par z.
c!liste d'appel
c
c      subroutine mzdivq(ichoix,nv,tv,nq,tq)
c
c     entree :
c     - ichoix. le nouveau reste ne sequential calculant
c          qu'avec le reste precedent, ce qui n'est pas le cas du
c          quotient, la possibilite est donnee de ne calculer que
c          ce reste. ichoix=1 .Si l'on desire calculer aussi le
c          quotient, ichoix=2.
c     - nv. est le degre du quotient entrant tv.
c     - tv. est le tableau contenant les coeff. du quotient.
c     - tr. est le tableau contenant les coeff. du reste de
c           degre nq-1.
c     - nq. est le degre du polynome tq.
c     - tq. est le tableau contenant les coeff. du pol. tq.
c
c     sortie :
c     - nv. est le degre du nouveau quotient.
c     - tv. contient les coeff. du nouveau quotient.
c     - tr. ceux du nouveau reste de degre toujours nq-1.
c!

      implicit double precision (a-h,o-y)
      dimension tv(0:*),tq(0:*)
c
      raux=tv(nq-1)
c
c     -- Calcul du nouveau reste -------------
c
      do 20 i=nq-1,1,-1
         tv(i)= tv(i-1) - tq(i)*raux
 20   continue
c
      tv(0)= -tq(0)*raux
c
      if (ichoix.eq.1) return
c
c     -- Calcul du nouveau quotient ----------
c
      do 30 i=nq+nv,nq,-1
         tv(i+1)=tv(i)
 30   continue
c
      tv(nq)=raux
      nv=nv+1
c
      return
      end
