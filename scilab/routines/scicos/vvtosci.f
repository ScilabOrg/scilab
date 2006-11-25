      subroutine vvtosci(x,nx)
c     fortran var2vec  to scilab
c     x is supposed to be a fortran image of var2vec result
c 
c     Copyright INRIA
      double precision x(*)
c 
      integer  l
c      integer  inull(4)
      double precision dnull(4)
c 
      external dcopy, error
      include "../stack.h"
c      data inull/1,1,0,0/
      data dnull/1.00d0,1.00d0,0.00d0,0.00d0/
c 
      if (top .ge. bot) then
        call error(18)
      else
        top = top + 1
        l=lstk(top)

       if (nx.eq.0) then
        err = l + 4 - lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
c          call dcopy(2,inull,1,stk(l),1)
          call dcopy(2,dnull,1,stk(l),1)
          lstk(top+1) = l + 4
       else
        err = l + nx - lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
        call dcopy(nx,x,1,stk(l),1)
        lstk(top+1) = l + nx
       endif
      endif
      end 
