c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine itosci(x,mx,nx)
C     fortran to scilab
C 
C.. Formal Arguments .. 
      integer  x(*)
      integer  mx
      integer  nx
C 
C.. Local Scalars .. 
      integer  il,l
C 
C.. External Calls .. 
      external error, int2db
C 
C.. Include Statements ..
      include "stack.h"
C 
C.. Statement Functions ..
      integer  iadr
      integer  sadr
      iadr(l) = l + l - 1
      sadr(l) = (l/2) + 1
C 
C ... Executable Statements ...
C 
C     
      m = mx
      n = nx
      if(mx*nx.eq.0) then
         n = 0
         m = 0
      endif

      if (top .ge. bot) then
        call error(18)
      else
        top = top + 1
        il = iadr(lstk(top))
        l = sadr(il+4)
        err = l + m*n - lstk(bot)
        if (err .gt. 0) then
          call error(17)
        else
          istk(il) = 1
          istk(il+1) = m
          istk(il+2) = n
          istk(il+3) = 0
          if(n.ne.0) call int2db(n*m,x,1,stk(l),1)
          lstk(top+1) = l + n*m
        endif
      endif
      end 
