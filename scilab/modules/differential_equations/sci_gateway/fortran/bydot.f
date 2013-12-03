c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine bydot(n,t,y,ydot)
c     
      INCLUDE 'stack.h'
      integer iadr,sadr
c     
      logical allowptr
      double precision t(*), y(*),ydot(*)
      integer vol,tops,nordre
      data nordre/1/,mlhs/1/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     
c     nordre=external number
c     mlhs (mrhs) = number ot output (input) parameters of the 
c     external 
c     
      ierror=0
      mrhs=2
c     
      ilp=iadr(lstk(top))
      il=istk(ilp+nordre)
c
      tops=istk(il)
      ils=iadr(lstk(tops))
c
      if(istk(ils).eq.10) then
c     fortran external
         call fydot(n,t,y,ydot)
         return
      endif

c     external is a Scilab function

c     on return ierror=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      ierror=1

c     
c     transfer of input parameters
c+    
      call ftob(t,1,istk(il+1))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(y,n,istk(il+2))
      if(err.gt.0.or.err1.gt.0) return

c+    
c     adress of external
      fin=lstk(tops)
c     
c     external in a list
      if(istk(ils).eq.15) then
      nelt=istk(ils+1)
      l=sadr(ils+3+nelt)
      ils=ils+2
c     
c     adress of external
      fin=l
c     
c     additional parameters
c     
      nelt=nelt-1
      if(nelt.ne.0) then
         l=l+istk(ils+1)-istk(ils)
         vol=istk(ils+nelt+1)-istk(ils+1)
         if(top+1+nelt.ge.bot) then
            call error(18)
            return
         endif
         err=lstk(top+1)+vol-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(vol,stk(l),1,stk(lstk(top+1)),1)
         do 11 i=1,nelt
            top=top+1
            lstk(top+1)=lstk(top)+istk(ils+i+1)-istk(ils+i)
 11      continue
         mrhs=mrhs+nelt
      endif
      endif
c     
c     execute scilab external
c     
      pt=pt+1
      if(pt.gt.psiz) then
         call  error(26)
         return
      endif
      ids(1,pt)=lhs
      ids(2,pt)=rhs
      rstk(pt)=1001
      lhs=mlhs
      rhs=mrhs
      niv=niv+1
      fun=0
c     
      icall=5

      include 'callinter.h.f'
c     
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
c     transfer of output parameters of external to fortran
      isComplex = istk(iadr(lstk(top))+3)
      if(isComplex.eq.1) then
        buf='ode: external must be real.'
        call error(9999)
        return
      endif
      call btof(ydot,n)
      if(err.gt.0.or.err1.gt.0) return
c     normal return ierror set to 0
      ierror=0
c+    
      return
c     
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end
