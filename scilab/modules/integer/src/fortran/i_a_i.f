c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine i_a_i
c     addition
      include 'stack.h'

Cc (DLL Intel Fortran)
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /mtlbc/
cDEC$ ENDIF
      common /mtlbc/ mmode

      integer is1
      external memused
      integer memused
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1     
c
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=il2+4
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=il1+4
      mn1=m1*n1


      if(istk(il1).ne.8.or.istk(il2).ne.8) then
         if((istk(il1).eq.1.and.it1.eq.0).or.
     &        (istk(il2).eq.1.and.it2.eq.0) ) goto 06
         top=top+1
         fin=-fin
         return
      endif


 06   if(it1.ne.it2.and.mn1.ne.0.and.mn2.ne.0) then
         top=top+1
         fin=-fin
         return
      endif


 07   continue
      if (mn1.eq.0) then
         if (mmode.eq.1) then
c     .     Matlab like []+a=[]
         else
c     .     []+a=a
            call icopy(4,istk(il2),1,istk(il1),1)
            call gencopy(it2,mn2,istk(l2),1,istk(l1),1)
            lstk(top+1)=sadr(l1+memused(it2,mn2))
         endif
      elseif (mn2.eq.0) then
         if (mmode.eq.1) then
c     .     Matlab like a+[]=[]
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
         else
c     .     a+[]=a
         endif
      elseif (m1 .lt. 0) then
c     .  eye+vector
c         go to 40
      elseif (m2 .lt. 0) then
c     .  vector+eye
c         go to 41
      elseif (mn2.eq.1) then
c     .  vector+const
         call genadd1(it1,mn1,istk(l2),0,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it1,mn1))
      elseif (mn1.eq.1) then
c     .  cst+vector
         call gencopy(it1,1,istk(l1),1,is1,1)
         call gencopy(it1,mn2,istk(l2),1,istk(l1),1)
         call genadd1(it1,mn2,is1,0,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it1,mn2))

         istk(il1+1)=m2
         istk(il1+2)=n2
      else
c     .  vector+vector
         if (m1 .ne. m2.or.n1 .ne. n2) then
            call error(8)
            return
         endif
         call genadd1(it1,mn1,istk(l2),1,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it1,mn1))
      endif
 999  return
      end

