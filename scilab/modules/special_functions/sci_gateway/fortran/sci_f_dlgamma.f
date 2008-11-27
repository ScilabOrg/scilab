c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     SCILAB function : dlgamma, fin = 8
      subroutine intsdlgamma(fname)
c     
      character*(*) fname
      include 'stack.h'
c     
      logical checkrhs,checklhs,getmat
      double precision psi
c
      rhs = max(0,rhs)
c     
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable x (number 1)
      if(.not.getmat(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
      if(m1*n1.eq.0) then
         top=top-rhs+1
         return
      endif
      if(it1.ne.0) then
         err=1
         call error(52)
         return
      endif
      do 10 i=0,n1*m1-1
         stk(lr1+i)=psi(stk(lr1+i))
 10   continue
c     
      return
      end

