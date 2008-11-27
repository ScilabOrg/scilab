c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intdiary

      INCLUDE 'stack.h'
c     
      integer mode(2),fd
      logical opened
      integer iadr,sadr
c

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.1) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif

      top = top-rhs+1
      il=iadr(lstk(top))

      if(abs(istk(il)).eq.1) then
         fd = int(stk(sadr(il+4)))
         if(fd.eq.0) then
            call clunit(-wio,buf,mode)
            goto 29
         endif
      endif

      call v2cunit(top,'wb',lunit,opened,ierr)
      if(ierr.lt.0) then
         call error(244)
         return
      elseif(ierr.gt.0) then
         return
      endif
c     
      if(wio.ne.0) then
         mode(1)=0
         mode(2)=0
         call clunit(-wio,buf,mode)
      endif
      if(lunit.eq.0) goto 29
      wio=lunit
      istk(il)=0
      goto 999
 29   wio=0
      istk(il)=0
      goto 999
 999  return
      end
