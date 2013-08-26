c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
       subroutine wspcle(ma,na,ar,ai,nela,inda,rr,ri,nelr,indr,
     $     abstol,reltol)
c     cleans a sparse matrix
c!
      integer inda(*),indr(*)
      integer ma,na,nela,nelr
      double precision ar(nela), ai(nela),rr(*),ri(*),abstol,reltol
      double precision amax,aa
c
      amax=0.0d0
      do 10 i=1,nela
         amax=max(amax,abs(ar(i))+abs(ai(i)))
 10   continue
c
      i0=0
      i1=i0
      i=1
      ni=inda(i)
      nelr=0
      do 40 k=1,nela
 38      i0=i0+1
         if(i0-i1.le.ni) goto 39
         i1=i0
         indr(i)=0
         i=i+1
         ni=inda(i)
         goto 38
 39      j=inda(ma+k)
         aa=abs(ar(k))+abs(ai(k))
         if(aa.ge.abstol.and.aa.gt.reltol*amax) then
            nelr=nelr+1
            indr(i)=indr(i)+1
            indr(ma+nelr)=inda(ma+k)
            rr(nelr)=ar(k)
            ri(nelr)=ai(k)
         endif
 40   continue
      end

