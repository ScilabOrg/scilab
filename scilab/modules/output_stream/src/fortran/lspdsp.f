c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine lspdsp(ne,ind,m,n,maxc,mode,ll,lunit,cw)
c!but
c     dspdsp visualise une matrice booleenne creuse
c!liste d'appel
c
c     subroutine lspdsp(ne,ind,m,n,maxc,mode,ll,lunit,cw)
c
c     double precision x(*)
c     integer ind(*)
c     integer nx,m,n,maxc,mode,ll,lunit
c     character cw*(*)
c
c     ind : indices specifiant la position des elements vrais
c     m : nombre de ligne de la matrice
c     n : nombre de colonnes de la matrice
c     maxc : nombre de caracteres maximum autorise pour
c            representer un nombre
c     mode : inutilise
c     ll : longueur de ligne maximum admissible
c     lunit : etiquette logique du support d'edition
c     cw : chaine de caracteres de travail de longueur au moins ll
c!
      integer maxc
      integer ind(*)
      character cw*(*)

c
      if(ne.eq.0) then
         write(cw,'(''('',i5,'','',i5,'') False sparse matrix'')') m,n
         call basout(io,lunit,cw(1:33))
         call basout(io,lunit,' ')
         goto 99
      else
         write(cw,'(''('',i5,'','',i5,'') sparse matrix'')') m,n
         call basout(io,lunit,cw(1:27))
         call basout(io,lunit,' ')
         if(io.eq.-1) goto 99
      endif
      ilr=1
      ilc=m+1
      cw=' '
      i0=0
      i1=i0
      l=1
      do 20 k=1,ne
         cw=' '
 11      i0=i0+1
         if(i0-i1.gt.ind(l)) then
            i1=i0
            l=l+1
            goto 11
         endif
         i=l
         j=ind(ilc-1+k)
         write(cw,'(''('',i5,'','',i5,'')    T'')') i,j
         l1=19

         call basout(io,lunit,cw(1:l1) )
         if (io.eq.-1) goto 99

   20 continue
 99   continue
c
      end
