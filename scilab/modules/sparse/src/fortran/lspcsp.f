c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine lspcsp(op,ma,na,nela,inda,mb,nb,nelb,indb,
     $     nelc,indc)
c!purpose
c     concatenation of two  sparse boolean matrices.
c!parameters 
c     op   : integer flag which specifies the concatenation to perform
c            op=0 : row concatenation
c            op=1 : column concatenation
c     ma,na: row and column dimension of the a matrix  
c     mb,nb: row and column dimension of the b matrix  
c     nela :integer: number of non zero elements of a
c     nelb :integer: number of non zero elements of b
c     nelc :integer: 
c                   on entry maximum number  of non zero elements of c
c                   on return number of non zero elements of c
c     inda : a matrix control data:
c            inda(i) 1<=i<=nr contains the number of ith row non zero elements
c             of a
c            inda(m+i) 1<=i<=nela column index of each non zero element
c     indb : b matrix control data:
c            indb(i) 1<=i<=nr contains the number of ith row non zero elements
c            of b
c             indb(m+i) 1<=i<=nelb column index of each non zero element
c     
c     indc : on return contains c matrix control data:
c            indc(i) 1<=i<=mc contains the number of ith row non zero elements
c            of c
c            indc(mc+i) 1<=i<=nelb column index of each non zero element
c     ierr : if non zero initial value of nelc is to small
c     !
      integer op,nela,inda(*),nelb,indb(*),nelc,indc(*)
c     
c     
      if(op.eq.0) then
c     row concatenation
         ia=1
         ib=1
         ic=1
         do 20 i=1,ma
            call icopy(inda(i),inda(ma+ia),1,indc(ma+ic),1)
            ic=ic+inda(i)
            ia=ia+inda(i)
            if (indb(i).gt.0) then
               do 10 k=1,indb(i)
                  indc(ma+ic)=indb(mb+ib)+na
                  ic=ic+1
                  ib=ib+1
 10            continue
            endif
            indc(i)=inda(i)+indb(i)
 20      continue
         nelc=nela+nelb
      else
c     column concatenation
        call icopy(ma,inda,1,indc,1)
        call icopy(nela,inda(ma+1),1,indc(ma+mb+1),1)
        call icopy(mb,indb,1,indc(ma+1),1)
        call icopy(nelb,indb(mb+1),1,indc(ma+mb+1+nela),1)
        nelc=nela+nelb
      endif
      return 
      end


