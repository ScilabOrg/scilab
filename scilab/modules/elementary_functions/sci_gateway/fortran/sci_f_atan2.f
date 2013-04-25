c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine intatan2(id)
      INCLUDE 'stack.h'
      integer id(nsiz),tops

      double precision sr,si
      integer iadr,sadr
      logical checkrhs, checklhs
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(0,rhs)
      if (.not.checkrhs(fname,2,2)) return
      if (.not.checklhs(fname,1,1)) return
      tops=top

      il=iadr(lstk(top+1-rhs))

      il1=il
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).ne.1) then
         call funnam(ids(1,pt+1),'atan2',il1)
         fun=-1
         return
      endif

      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      l2=sadr(il2+4)
      top=top-1
      if(istk(il2).ne.1) then
         top=tops
         call funnam(ids(1,pt+1),'atan2',il1)
         fun=-1
         return
      endif
      il1=iadr(lstk(top))

      if(istk(il1).lt.0) then
c     .  First argument is passed by reference
         ilr=il1
         il1=iadr(istk(il1+1))
         mn1=istk(il1+1)*istk(il1+2)
         it1=istk(il1+3)
         l1=sadr(il1+4)
         lr=sadr(ilr+4)
         err=lr+mn1*(it1+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il1),1,istk(ilr),1)
         lstk(top+1)=lr+mn1
      else
c     .  Argument is passed by value
         mn1=istk(il1+1)*istk(il1+2)
         it1=istk(il1+3)
         l1=sadr(il1+4)
         lr=l1
      endif
      if(istk(il2+3).ne.0.or.it1.ne.0) then
         call error(43)
         return
      endif
      if(istk(il2+1)*istk(il2+2).ne.mn1) then
         call error(60)
         return
      endif
      if(mn1.eq.0) return
      do 20 i=0,mn1
c         if(abs(stk(l1+i))+abs(stk(l2+i)).eq.0.0d+0) then
c            if(ieee.eq.0) then
c               call error(32)
c               return
c            elseif(ieee.eq.1) then
c               call msgs(64)
c            endif
c         endif
         stk(lr+i)=atan2(stk(l1+i),stk(l2+i))
 20   continue
      return
      end
c     -------------------------------
