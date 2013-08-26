c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine lspmat(ma,na,nela,inda,mr,indr,iw)
c     lspmat reshape a sparse boolean matrix
      integer inda(*),indr(*),iw(*)
      integer ma,na,nela
c
      nr=(ma*na)/mr
      ja=0
      ki=1
      kj=nela+1
      do 20 i=1,ma
         nira=inda(i)
         if(nira.ne.0) then
            do 10 j=1,nira
               ija=i-1+(inda(ma+ja+j)-1)*ma
               iw(kj)=int(ija/mr)+1
               iw(ki)=ija-mr*(iw(kj)-1)+1
               ki=ki+1
               kj=kj+1
 10         continue
            ja=ja+nira
         endif
 20   continue
c     lij2sp     (m, n, nel, ij,v,ind,nind,  iw,          ierr)
      call lij2sp1(mr,nr,nela,iw,indr,mr+nela,iw(2*nela+1),ierr)
      end
