c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine ftob(x,nx,topx)
      include 'stack.h'
      integer iadr,sadr
c
c     fortran to scilab
      integer topx,hsize
      double precision x
      dimension x(*)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      ilx=iadr(lstk(topx))
      hsize=4
      if(istk(ilx).eq.2) hsize=9+istk(ilx+1)*istk(ilx+2)
      if(top.ge.bot) then
         call error(18)
         return
      endif
      top=top+1
      il=iadr(lstk(top))
      err=lstk(top)+sadr(hsize)+nx-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call icopy(hsize,istk(ilx),1,istk(il),1)
      l=sadr(il+hsize)
      call unsfdcopy(nx,x,1,stk(l),1)
      lstk(top+1)=l+nx
      return
      end
