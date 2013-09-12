c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine i_t
c     subtraction
      include 'stack.h'
      
Cc (DLL Intel Fortran)
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /mtlbc/
cDEC$ ENDIF
      common /mtlbc/ mmode
      integer vol
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     

      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=il1+4
      mn1=m1*n1

 70   if(abs(m1).ne.1.and.abs(n1).ne.1) then
         vol=mn1
         ll = iadr(lstk(top+1))
         err = sadr(ll+memused(it1,vol)) - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         call gencopy(it1,vol,istk(l1),1,istk(ll),1)
         call genmtran(it1,istk(ll),m1,istk(l1),n1,m1,n1)
      endif
      istk(il1+1)=n1
      istk(il1+2)=m1
      return
      end
