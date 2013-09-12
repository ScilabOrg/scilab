c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine lsposp(op,ma,na,nela,inda,mb,nb,nelb,indb,
     $     nelc,indc,ierr)
c!purpose
c     compare the elements of two  sparse boolean matrices.
c!parameters 
c     op   : integer flag which specifies the comparison to perform
c            see routine dcompa for more precision
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
c            indc(i) 1<=i<=nr contains the number of ith row non zero elements
c            of c
c            indc(m+i) 1<=i<=nelb column index of each non zero element
c     ierr : if non zero initial value of nelc is to small
c     !
      integer op,nela,inda(*),nelb,indb(*),nelc,indc(*),ierr
c     
      integer jc,ka,kb,jb,i,ja,j1,j2
      integer t
      logical lcompa,z
      external lcompa
c     
      nr=max(ma,mb)
      nc=max(na,nb)
c     
      nelmx=nelc
      ierr=0

c     jc counts elements of c.
      jc     = 1
c     ka,kb are numbers in first i rows of a,b.
      ka     = 1
      kb     = 1
      kc     = 1
c     jb counts elements of b.
      jb     = 1
c     i counts rows of a,b,c.
      if(ma*na.eq.1.and.mb*nb.gt.1) then
c     compare all element of b with scalar a
         t=0
         if(inda(1).eq.1) t=1
         z=lcompa(t,0,op)   
         do 10 i=1,nr
            indc(i)=0
            nirb=indb(i)
            jb=kb        
            jc=kc
            if(nirb.eq.0) then
               do 03 j=1,nc
                  if (z) then
                     if(jc+1.gt.nelmx) goto 99
                     indc(nr+jc)=j
                     jc=jc+1
                  endif
 03            continue
            else
               j2=indb(nr+jb)
               do 04 j=1,nc
                  if(j2.eq.j) then
                     if (lcompa(t,1,op)) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif 
                     if(jb-kb+1.lt.nirb) jb=jb+1
                     j2=indb(nr+jb)
                  else
                     if (z) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif 
                  endif
 04            continue
            endif
            kb=kb+indb(i)
            indc(i)=jc-kc
            kc=jc
 10      continue

      elseif(ma*na.gt.1.and.mb*nb.eq.1) then
c     compare all elements of a with scalar b  
         t=0
         if(indb(1).eq.1) t=1
         z=lcompa(0,t,op)
         do 20 i=1,nr
            indc(i)=0
            nira=inda(i)
            ja=ka       
            jc=kc
            if(nira.eq.0) then
               if(z) then
                  if(kc+nc.gt.nelmx) goto 99
                  indc(i)=nc
                  do 11 j=1,nc
                     indc(nr+kc-1+j)=j
 11               continue
                  jc=kc+nc
               endif
            else
               j1=inda(nr+ja)
               do 12 j=1,nc
                  if(j1.eq.j) then
                     if (lcompa(1,t,op)) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif
                     if(ja-ka+1.lt.nira) ja=ja+1
                     j1=inda(nr+ja)
                  elseif(z) then
                     if(jc+1.gt.nelmx) goto 99
                     indc(nr+jc)=j
                     jc=jc+1
                  endif
 12            continue
            endif
            indc(i)=jc-kc
            ka=ka+nira
            kc=jc
 20      continue 
      else
         z=lcompa(0,0,op)   
         do 30 i=1,nr
            indc(i)=0
            nira=inda(i)
            nirb=indb(i)
            ja=ka
            jb=kb        
            jc=kc
            if(nira.eq.0) then
               if(nirb.eq.0) then
                  if(z) then
                     if(kc+nc.gt.nelmx) goto 99
                     indc(i)=nc
                     do 21 j=1,nc
                        indc(nr+kc-1+j)=j
 21                  continue
                     jc=kc+nc
                  endif
               else
                  j2=indb(nr+jb)
                  do 22 j=1,nc
                     if(j2.eq.j) then
                        if (lcompa(0,1,op)) then
                           if(jc+1.gt.nelmx) goto 99
                           indc(nr+jc)=j
                           jc=jc+1
                        endif
                        if(jb-kb+1.lt.nirb) jb=jb+1
                        j2=indb(nr+jb)
                     elseif(z) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif
 22               continue
               endif
            else
               if(nirb.eq.0) then
                  j1=inda(nr+ja)
                  do 23 j=1,nc
                     if(j1.eq.j) then
                        if (lcompa(1,0,op)) then
                           if(jc+1.gt.nelmx) goto 99
                           indc(nr+jc)=j
                           jc=jc+1
                        endif
                        if(ja-ka+1.lt.nira) ja=ja+1
                        j1=inda(nr+ja)
                     elseif(z) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif
 23               continue
               else
                  j1=inda(nr+ja)
                  j2=indb(nr+jb)
                  do 24 j=1,nc
                     if(j1.eq.j) then
                        if(j2.eq.j) then
                           if (lcompa(1,1,op)) then
                              if(jc+1.gt.nelmx) goto 99
                              indc(nr+jc)=j
                              jc=jc+1
                           endif 
                           if(ja-ka+1.lt.nira) ja=ja+1
                           if(jb-kb+1.lt.nirb) jb=jb+1
                           j1=inda(nr+ja)
                           j2=indb(nr+jb)
                        else
                           if (lcompa(1,0,op)) then
                              if(jc+1.gt.nelmx) goto 99
                              indc(nr+jc)=j
                              jc=jc+1
                           endif 
                           if(ja-ka+1.lt.nira) ja=ja+1
                           j1=inda(nr+ja)
                        endif
                     else
                        if(j2.eq.j) then
                           if (lcompa(0,1,op)) then
                              if(jc+1.gt.nelmx) goto 99
                              indc(nr+jc)=j
                              jc=jc+1
                           endif
                           if(jb-kb+1.lt.nirb) jb=jb+1
                           j2=indb(nr+jb)
                        elseif(z) then
                           if(jc+1.gt.nelmx) goto 99
                           indc(nr+jc)=j
                           jc=jc+1
                        endif 
                     endif
 24               continue
               endif
            endif
            ka=ka+inda(i)
            kb=kb+indb(i)
            indc(i)=jc-kc
            kc=jc
 30      continue
      endif
      nelc  = jc-1
      return
c     error messages.
 99   ierr=1
c     no more place for c

      return 
      end


