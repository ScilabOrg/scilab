c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1989 - INRIA - Serge STEER
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine wwpow(n,vr,vi,iv,powr,powi,ierr)
c!but
c     eleve les elements d'un vecteur complexe a une puissance complexe
c!liste d'appel
c     subroutine wwpow(n,vr,vi,iv,powr,powi,ierr)
c     integer n,iv,ierr
c     double precision vr(n*iv),vi(n*iw),powr,powi
c
c     n : nombre d'element du vecteur
c     vr : tableau contenant les parties reelles des elements du vecteur
c     vi : tableau contenant les parties imaginaires des elements du vecteur
c     iv : increment entre deux element consecutif du vecteur dans le
c          tableau v
c     powr : partie reelle de la puissance a la quelle doivent etre
c            eleves les elements du vecteur
c     powi : partie imaginaire de la puissance a la quelle doivent etre
c            eleves les elements du vecteur
c     ierr : indicateur d'erreur
c            ierr=0 si ok
c            ierr=1 si 0**0
c            ierr=2 si 0**k avec k<0
c!origine
c Serge Steer INRIA 1989
c
      integer n,iv,ierr
      double precision vr(*),vi(*),powr,powi,sr,si
c
      ierr=0
c
      if(powi.ne.0.0d+0) goto 01
c puissance reelle
      call wdpow(n,vr,vi,iv,powr,ierr)
      return
c
   01 continue
c puissance complexes
      ii=1
      do 20 i=1,n
        if(abs(vr(ii))+abs(vi(ii)).ne.0.0d+0) then
                 call wlog(vr(ii),vi(ii),sr,si)
                 call wmul(sr,si,powr,powi,sr,si)
                 sr=exp(sr)
                 vr(ii)=sr*cos(si)
                 vi(ii)=sr*sin(si)
                 ii=ii+iv
                                             else
                 ierr=0
                 return
        endif
   20 continue
c
      return
      end
