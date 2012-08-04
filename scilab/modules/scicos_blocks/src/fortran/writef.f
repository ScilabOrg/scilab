c     Scicos 
c 
c     Copyright (C) INRIA - METALAU Project <scicos@inria.fr> 
c 
c     This program is free software; you can redistribute it and/or modify 
c     it under the terms of the GNU General Public License as published by 
c     the Free Software Foundation; either version 2 of the License, or 
c     (at your option) any later version. 
c 
c     This program is distributed in the hope that it will be useful, 
c     but WITHOUT ANY WARRANTY; without even the implied warranty of 
c     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
c     GNU General Public License for more details. 
c 
c     You should have received a copy of the GNU General Public License 
c     along with this program; if not, write to the Free Software 
c     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. 
c 
c     See the file ./license.txt 
c 

      subroutine writef(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     write input to a binary or formatted file
c     ipar(1) = lfil : file name length
c     ipar(2) = lfmt : format length (0) if binary file
c     ipar(3)          unused
c     ipar(4) = N : buffer length
c     ipar(5:4+lfil) = character codes for file name
c     ipar(5+lfil:4+lfil+lfmt) = character codes for format if any
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny


      include 'stack.h'
      integer i, n, jj
      integer mode(2)
      integer fmttyp
c     
      N=ipar(4)
      K=int(z(1))
      lunit=int(z(2))
c
      if(flag.eq.2.and.nevprt.gt.0) then
c     add new point to the buffer
         K=K+1
         z(2+K)=t
         do 01 i=1,nu
            z(2+N+(i-1)*N+K)=u(i)
 01      continue
         z(1)=K
         if(K.lt.N) return
c     write on the file
         jj=fmttyp(ipar(5+ipar(1)),ipar(2))
         if (fmttyp(ipar(5+ipar(1)),ipar(2)).eq.0) GOTO 100
         if (ipar(2).gt.0) then
c     .     formatted write
            call cvstr(ipar(2),ipar(5+ipar(1)),buf,1)
            do 10 j=1,K
               write(lunit,buf(1:ipar(2)),err=100)
     $              (z(2+N+(i-1)*N+j),i=0,nu)
 10         continue
         else
c     .     unformatted write
            do 11 j=1,K
               write(lunit,err=100) (z(2+N+(i-1)*N+j),i=0,nu)
 11         continue
         endif
         z(1)=0.0d0
      elseif(flag.eq.4) then
c     file opening
         if (fmttyp(ipar(5+ipar(1)),ipar(2)).le.0) GOTO 110
         lfil=ipar(1)
         call cvstr(lfil,ipar(5),buf,1)
         lfmt=ipar(2)
         lunit=0
         if(lfmt.gt.0) then
            mode(1)=003
            mode(2)=0
            call clunit(lunit,buf(1:lfil),mode)
            if(err.gt.0) goto 100
         else
            mode(1)=103
            mode(2)=0
            call clunit(lunit,buf(1:lfil),mode)
            if(err.gt.0) goto 100
         endif
         z(1)=0
         z(2)=lunit
         z(3)=t
         call dset(nu*N,0.0d0,z(4),1)
      elseif(flag.eq.5) then
         if(lunit.eq.0) then
            return
         endif
         if(K.ge.1) then
c     write on the file
            lfmt=ipar(2)
            if (lfmt.gt.0) then
c     .        formatted write
               call cvstr(lfmt,ipar(5+ipar(1)),buf,1)
               do 20 j=1,K
                  write(lunit,buf(1:lfmt)) (z(2+N+(i-1)*N+j),i=0,nu)
 20            continue
            else
c     .        unformatted write
               do 21 j=1,K
                  write(lunit) (z(2+N+(i-1)*N+j),i=0,nu)
 21            continue
            endif
         endif
         lfil=ipar(1)
         call clunit(-lunit,buf(1:lfil),mode)
         if(err.gt.0) goto 100
         z(2)=0.0d0
      endif
      return
 100  continue
      err=0
      call basout(io,wte,'File '//buf(1:lfil)//' Cannot be opened')
 110  continue      
      flag=-1
      return
      end
