c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$
      subroutine intdggbal(fname)
c     [Ab,Bb,X,Y]=balanc(A,B)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character JOB
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )

      minrhs=2
      maxrhs=2
      minlhs=4
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif
      if(.not.getrhsvar(2,'d', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.MB) then
         call error(267)
         return
      endif
      N = MA
      if(N.eq.0) then
         if(.not.createvar(3,'d', N, N, lX)) return
         if(.not.createvar(4,'d', N, N, lY)) return
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=3
         lhsvar(4)=4
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif

      if(.not.createvar(3,'d', N, N, lX)) return
      if(.not.createvar(4,'d', N, N, lY)) return
      if(.not.createvar(5,'d', 1, N, lLSCALE)) return
      if(.not.createvar(6,'d', 1, N, lRSCALE)) return
      if(.not.createvar(7,'d', 1, N*6, lWORK)) return


c     SUBROUTINE DGGBAL( JOB, N, A, LDA, B, LDB, ILO, IHI, LSCALE,
c     $                   RSCALE, WORK, INFO )

      JOB='B'
      call DGGBAL(JOB, N, stk(lA), N, stk(lB), N, ILO, IHI,stk(lLSCALE),
     $     stk(lRSCALE), stk(lWORK), INFO )

c     
      call DLASET( 'F', N, N, ZERO, ONE, stk(lX), N ) 
      call DLASET( 'F', N, N, ZERO, ONE, stk(lY), N ) 

      call DGGBAK(JOB, 'L', N, ILO, IHI, stk(lLSCALE), stk(lRSCALE), N,
     $     stk(lX), N, INFO )
      call DGGBAK(JOB, 'R', N, ILO, IHI, stk(lLSCALE), stk(lRSCALE), N,
     $     stk(lY), N, INFO )


      lhsvar(1)=1
      lhsvar(2)=2
      lhsvar(3)=3
      lhsvar(4)=4
      end


