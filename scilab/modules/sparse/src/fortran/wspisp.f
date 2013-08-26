c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine wspisp(ma,na,ar,ai,nela,inda,i,ni,j,nj,
     $     mb,nb,br,bi,nelb,indb,mr,nr,rr,ri,nelr,indr,ptrb,
     $     ierr,ita,itb)
c     extract a submatrix from a sparse matrix
c!
      integer inda(*),indr(*),i(*),j(*),ptrb(*)
      integer indb(*),mb,nb,nelb
      integer ma,na,ni,nj,mr,nr,nela,nelr,ierr
      double precision ar(nela),ai(nela),rr(*),ri(*),br(nelb),bi(nelb)
      logical allrow,allcol
      integer findl
      external findl
c
      mr=ni
      nr=nj
      ierr=0
      nelmx=nelr
      allrow=ni.lt.0
      allcol=nj.lt.0
      if(allrow) then 
         mr=ma
         ni=mr
      else
         mi=0
         do 01 kk=1,ni
            mi=max(mi,i(kk))
 01      continue
         mr=max(ma,mi)
      endif
      if(allcol) then 
         nr=na
         nj=na
      else
         mj=0
         do 02 kk=1,nj
            mj=max(mj,j(kk))
 02      continue
         nr=max(na,mj)
      endif
      if (allrow.and.allcol) then
c     a(:,:)=b
         call icopy(mb+nelb,indb,1,indr,1)
         call unsfdcopy(nelb,br,1,rr,1)
         if(itb.ne.0) then
            call unsfdcopy(nelb,bi,1,ri,1)
         else 
            call dset(nelb,0.0d0,ri,1)
         endif
         nelr=nelb
         return
      elseif(allcol) then
c     a(i,:)=b
         jr=1
         jb=1
         ja=1
         call sz2ptr(indb,mb,ptrb)
         do 20 l=1,mr
            indr(l)=0
            ii=findl(l,i,ni)
            if(ii.eq.0) then
c     this line is not modified
               if(l.le.ma) then
                  indr(l)=inda(l)
                  call icopy(indr(l),inda(ma+ja),1,indr(mr+jr),1)
                  if(jr+indr(l).gt.nelmx) then
                     ierr=1
                     return
                  endif
                  call unsfdcopy(indr(l),ar(ja),1,rr(jr),1)
                  if(ita.ne.0) then
                     call unsfdcopy(indr(l),ai(ja),1,ri(jr),1)
                  else                     
                     call dset(indr(l),0.0d0,ri(jr),1)
                  endif
                  jr=jr+indr(l)
                  ja=ja+indr(l)
               else
                  indr(l)=0
               endif
            else
c     all this line is replaced by corresponding b line
               jb=ptrb(ii)
               indr(l)=indb(ii)
               if(jr+indr(l).gt.nelmx) then
                  ierr=1
                  return
               endif
               call icopy(indr(l),indb(mb+jb),1,indr(mr+jr),1)
               call unsfdcopy(indr(l),br(jb),1,rr(jr),1)
               if(itb.ne.0) then
                  call unsfdcopy(indr(l),bi(jb),1,ri(jr),1)
               else
                  call dset(indr(l),0.0d0,ri(jr),1)
               endif
               jr=jr+indr(l)
               if(l.le.ma) then
                  ja=ja+inda(l)
               endif
            endif
 20      continue
         nelr=jr-1
      elseif(allrow) then
c     a(:,j)=b
         jr=1
         ja=0
         jb=0
         do 35 l=1,ma
            ja1=1
            nbl=indb(l)
            nal=inda(l)
            indr(l)=0
            do 31 k=1,nr
               jj=findl(k,j,nj)
               if(jj.eq.0) then
c     the  a(l,k) element is not modified insert it in r if non zero
                  if(ja1.le.nal.and.inda(ma+ja+ja1).eq.k) then
c     *              a(l,k) element is non zero
                     if(jr+1.gt.nelmx) then
                        ierr=1
                        return
                     endif
                     rr(jr)=ar(ja+ja1)
                     if(ita.ne.0) then
                        ri(jr)=ai(ja+ja1)
                     else
                        ri(jr)=0.0d0
                     endif
                     indr(l)=indr(l)+1
                     indr(mr+jr)=k
                     jr=jr+1
                     ja1=ja1+1
                  endif
               else
c     the  a(l,k) element is replaced by b(l,jj) element if non zero
                  jb1=findl(jj,indb(mb+jb+1),nbl)
                  if(jb1.ne.0) then
c     *           b(l,jj) element if non zero
                     if(jr+1.gt.nelmx) then
                        ierr=1
                        return
                     endif
                     rr(jr)=br(jb+jb1)
                     if(itb.ne.0) then
                        ri(jr)=bi(jb+jb1)
                     else
                        ri(jr)=0.0d0
                     endif
                     indr(l)=indr(l)+1
                     indr(mr+jr)=k
                     jr=jr+1
                  endif
                  if(ja1.le.nal.and.inda(ma+ja+ja1).eq.k) ja1=ja1+1
               endif
 31         continue
            ja=ja+nal
            jb=jb+nbl
 35     continue
        nelr=jr-1
        return
      else
c     a(i,j)=b
         jr=1
         ja=0
         call sz2ptr(indb,mb,ptrb)
         do 45 l=1,mr
            ja1=1
            if(l.le.ma) then 
               nal=inda(l)
            else
               nal=0
            endif
            indr(l)=0
            ii=findl(l,i,ni)
            if(ii.eq.0) then
c     *     the a(l,:) is not modified
               if(l.le.ma) then
                  indr(l)=inda(l)
                  call icopy(indr(l),inda(ma+ja+ja1),1,indr(mr+jr),1)
                  call unsfdcopy(indr(l),ar(ja+ja1),1,rr(jr),1)
                  if(ita.ne.0) then
                     call unsfdcopy(indr(l),ai(ja+ja1),1,ri(jr),1)
                  else
                     call dset(indr(l),0.0d0,ri(jr),1)
                  endif
                  jr=jr+indr(l)
                  ja1=ja1+indr(l)
               else
                  indr(l)=0
               endif
            else
               jb=ptrb(ii)-1
               jb1=1
               nbl=indb(ii)
               do 42 k=1,nr
                  jj=findl(k,j,nj)
                  if(jj.eq.0) then
c     *           insert a(l,k) element insert  in r if non zero
                     if(ja1.le.nal.and.inda(ma+ja+ja1).eq.k) then
c     *              a(l,k) element is non zero
                        if(jr+1.gt.nelmx) then
                           ierr=1
                           return
                        endif
                        rr(jr)=ar(ja+ja1)
                        if(ita.ne.0) then
                           ri(jr)=ai(ja+ja1)
                        else
                           ri(jr)=0.0d0
                        endif
                        indr(l)=indr(l)+1
                        indr(mr+jr)=k
                        jr=jr+1
                        ja1=ja1+1
                     endif
                  else
c     *           replace a(l,k) element  by b(ii,jj) element if non zero
                     jb1=findl(jj,indb(mb+jb+1),nbl)
                     if(jb1.ne.0) then
c     *               b(l,jj) element if non zero
                        if(jr+1.gt.nelmx) then
                           ierr=1
                           return
                        endif
                        rr(jr)=br(jb+jb1)
                        if(itb.ne.0) then
                           ri(jr)=bi(jb+jb1)
                        else
                           ri(jr)=0.0d0
                        endif
                        indr(l)=indr(l)+1
                        indr(mr+jr)=k
                        jr=jr+1
                     endif
                     if(ja1.le.nal.and.inda(ma+ja+ja1).eq.k) ja1=ja1+1
                  endif
 42            continue
            endif
c     bug fixed by Ramine
c            ja=ja+na
            ja=ja+nal
 45      continue
         nelr=jr-1
      endif
      end
