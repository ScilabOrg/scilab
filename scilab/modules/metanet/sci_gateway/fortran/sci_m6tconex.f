
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6tconex, fin = 23
       subroutine intsm6tconex(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1

       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable la (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable lp (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable ls (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n3)) return
       nn1= int(stk(lr4))
       if(.not.cremat(fname,top+1,0,nn1,1,lw1,loc1)) return
       if(.not.cremat(fname,top+2,0,1,1,lw2,loc2)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+3,0,istk(iadr(lr4)),1,lw8,loc8)) return
       call tconex(stk(lw1),stk(lw2),istk(iadr(lr1)),istk(iadr(lr2)),ist
     $ k(iadr(lr3)),n1,istk(iadr(lr4)),stk(lw8))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: iscon
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw2)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
