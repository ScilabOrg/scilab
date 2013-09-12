c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intlib(lendirlib, libdir)
      INCLUDE 'stack.h'
      CHARACTER*(*)  libdir
      integer lendirlib
      integer percen,mode(2),id(nlgh)
      integer iadr,sadr
c
      data percen/56/
      data nclas/29/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c

c     path  du repertoire
      il=iadr(lstk(top))
      n=lendirlib
      call cvstr(lendirlib,istk(il+2),libdir,0)

      istk(il+1)=lendirlib
      istk(il)=14
      il0=il+2+lendirlib
      ilc=il0+1
      iln=ilc+nclas+1
c
c     ouverture du fichier names
      call cvstr(n,istk(il+2),buf,1)
      buf=libdir(1:n)//'names'
      mode(1)=-1
      lunit=0
      len = n + 5
      call getshortpathname(buf(1:len),len)
      call clunit(lunit,buf(1:len),mode)
      if(err.gt.0) then
         buf(n+6:)=' '
         call error(err)
         return
      endif
      m=0
      il=iln
c     lecture des noms
 131  read(lunit,'(a)',err=139,end=132) buf
      err=sadr(il+nsiz)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call cvname(istk(il),buf(1:nlgh),0)
      il=il+nsiz
      m=m+1
      goto 131
 132  continue
      mode(1)=0
      mode(2)=0
      call clunit(-lunit,' ',mode)
c     tri dans l'ordre alphabetique
      il2=il+1
      err=sadr(il2+(nsiz+1)*m)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call icopy(nsiz*m,istk(iln),1,istk(il2),1)
      call iset(nclas+1,0,istk(ilc),1)
      il=iln
      il1=il
      do 133 i=1,m
         call namstr(istk(il),id,nn,1)
         ic=abs(id(1))
         if(ic.eq.percen) then
            ic=abs(id(2))
         endif
         ic=min(nclas,max(1,ic-9))
         istk(ilc+ic)=istk(ilc+ic)+1
         istk(il1)=ic
         il=il+nsiz
         il1=il1+1
 133  continue
      il1=il2+m*nsiz
      call isort(istk(iln),m,istk(il1))
      il1=il1+m
      il=iln
      do 134 i=1,m
         il1=il1-1
         ic=istk(il1)
         call putid(istk(il),istk(il2+nsiz*(ic-1)))
         il=il+nsiz
 134  continue
c     table des pointeurs
      istk(ilc)=1
      do 135 i=1,nclas
         istk(ilc+i)=istk(ilc+i-1)+istk(ilc+i)
 135  continue
      istk(il0)=m
      lstk(top+1)=sadr(iln+m*nsiz)
      goto 999
c
 139  call error(49)
      call clunit(-lunit,buf,mode)
 999  return
      end
