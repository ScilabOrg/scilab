c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1989 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine dipow(n,v,iv,ipow,ierr)
c!but
c     eleve a une puissance entiere les elements d'un vecteur de flottants
c     double precision
c!liste d'appel
c     subroutine dipow(n,v,iv,ipow,ierr)
c     integer n,iv,ipow ,ierr
c     double precision v(n*iv)
c
c     n : nombre d'element du vecteur
c     v : tableau contenant les elements du vecteur
c     iv : increment entre deux element consecutif du vecteur dans le
c          tableau v
c     ipow : puissance a la quelle doivent etre eleves les elements du
c            vecteur
c     ierr : indicateur d'erreur
c            ierr=0 si ok
c            ierr=1 si 0**0
c            ierr=2 si 0**k avec k<0
c!origine
c Serge Steer INRIA 1989
c
      integer n,iv,ipow ,ierr
      double precision v(*)
c
      ierr=0
c
      if(ipow.eq.1) return
      if(ipow.eq.0) then
c     puissance 0
         ii=1
         do 10 i=1,n
            if(v(ii).ne.0.0d+0) then
               v(ii)=1.0d+0
               ii=ii+iv
            else
               ierr=1
               return
            endif
 10      continue
      else if(ipow.lt.0) then
c     puissance negative
         ii=1
         do 20 i=1,n
            if(v(ii).ne.0.0d+0) then
               v(ii)=v(ii)**ipow
               ii=ii+iv
            else
               ierr=2
               return
            endif
 20      continue
      else
c     puissance  positive
         ii=1
         do 30 i=1,n
            v(ii)=v(ii)**ipow
            ii=ii+iv
 30      continue
      endif
c     
      return
      end
