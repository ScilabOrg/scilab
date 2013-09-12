c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine dperm(vv,nv,ind)
c apply an in-situ  permutation 
      double precision vv(nv),x
      integer ind(nv)
c
      x=vv(1)
      i3=1
      i0=1
      i=i0
 10   continue
      if(ind(i).eq.i0) then 
c     end of a permutation cycle 
        vv(i)=x
        ind(i)=-ind(i)
c       look for first positive ind(i)
        i0=i3
 11     i0=i0+1
        if(i0.gt.nv) goto 20
        if(ind(i0).lt.0) goto 11
c       beginning of a permutation cycle 
        i3=i0
        i=i0
        x=vv(i0)
      else
         vv(i)=vv(ind(i))
         ind(i)=-ind(i)
         i=-ind(i)
      endif
      goto 10
 20   continue

      do 30 i=1,nv
         ind(i)=-ind(i)
 30   continue
      end
