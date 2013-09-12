c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine wspxsp(nr,nc,ar,ai,nela,inda,br,bi,nelb,indb,cr,ci,
     $     nelc,indc,ierr,it1,it2)
c!purpose
c     multiply two sparse matrices elementwise
c!parameters     
c     a,b,c : arrays. 
c             Contain non zero elements of first,second and sum matrices.
c     nr : integer: row dimension of a b c matrices
c     nc : integer: column dimension of a b c matrices
c     nela :integer: number of non zero elements of a
c     nelb :integer: number of non zero elements of b
c     nelc :integer: 
c            on entry maximum number  of non zero elements of c
c            on return number of non zero elements of c
c     inda : a matrix control data:
c            inda(i) 1<=i<=nr contains the number of ith row non zero elements
c            of a
c            inda(m+i) 1<=i<=nela column index of each non zero element
c     indb : b matrix control data:
c            indb(i) 1<=i<=nr contains the number of ith row non zero elements
c            of b
c            indb(m+i) 1<=i<=nelb column index of each non zero element
c
c     indc : on return contains c matrix control data:
c            indc(i) 1<=i<=nr contains the number of ith row non zero elements
c            of c
c            indc(m+i) 1<=i<=nelb column index of each non zero element
c     ierr : if non zero initial value of nelc is to small
c!
      double precision ar(nela),ai(nela),br(nelb),bi(nelb)
      double precision cr(*),ci(*)
      integer nr,nc,nela,inda(*),nelb,indb(*),nelc,indc(*),ierr
c
      integer jc,ka,kb,jb,kf,i,ka1,ja,j1,j2,nold
c     
      nelmx=nelc
      ierr=0
c     jc counts elements of c.
      jc     = 1
c     ka,kb are numbers in first i rows of a,b.
      ka     = 0
      kb     = 0
c     kf is number of control data in a,b or c.
      kf     = nr
c     jb counts elements of b.
      jb     = 1
c     i counts rows of a,b,c.
      do 15 i=1,nr
         kb      = kb+indb(i)
c     nira is number in row i of a.
         nira    = inda(i)
         if (nira.eq.0) go to 12
         ka1     = ka+1
         ka      = ka+nira
c     ja counts elements of a.
         do 11 ja= ka1,ka
 6          j1     = inda(ja+kf)
c     at end of b-row transfer rest of a-row.
            if (jb.gt.kb) go to 11
            j2     = indb(jb+kf)
            if ((j1-j2) .lt. 0) then
               goto 11
            elseif ((j1-j2) .eq. 0) then
               goto 9
            else
               goto 10               
            endif
c     if a-index equals b-index multiply elements ,place  in c.
 9          continue
            if (jc.gt.nelmx) go to 16
            if(it1.eq.0) then
               cr(jc)  = ar(ja)*br(jb)
               ci(jc)  = ar(ja)*bi(jb)
            elseif(it2.eq.0) then
               cr(jc)  = ar(ja)*br(jb)
               ci(jc)  = ai(ja)*br(jb)
            else
               cr(jc)  = ar(ja)*br(jb)-ai(ja)*bi(jb)
               ci(jc)  = ai(ja)*br(jb)+ar(ja)*bi(jb)
            endif
            indc(jc+kf)=j1
            jc     = jc+1
            jb     = jb+1
            go to 11
c     if a-index greater than b-index 
 10         jb     = jb+1
            go to 6
 11      continue
c     end of row of a.  transfer rest of row of b.
 12      jb=kb+1
         if (i.gt.1) go to 14
         nold  = jc-1
c     nirc is number in row i of c.
         nirc  = jc-1
         go to 15
 14      nirc   = jc-1-nold
         nold  = jc-1
 15      indc(i)=nirc
         nelc  = jc-1
         return
c     error messages.
 16      ierr=1
c     no more place for c

         return 
         end
