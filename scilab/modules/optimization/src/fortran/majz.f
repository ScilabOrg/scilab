c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine majz(n,np,nt,y,s,z,ys,zs,diag,index)
c
c     mise a jour de ({z}(i),zs(i), i=1,np).
c     {z}(i)=[b](i-1)*{s}(i), [b](i) est definie par ({y}(j),{s}(j),{z}(j)
c     , j=1,i) et {diag}.
c     zs(i)=<z>(i)*{s}(i)
c
      implicit double precision (a-h,o-z)
      dimension     y(nt,n),s(nt,n),z(nt,n),ys(nt),zs(nt),diag(n)
      integer  index(nt)
c
      l=index(1)
      do 100 jj=1,n
         z(l,jj)=diag(jj)*s(l,jj)
100   continue
c
      zs(l)=0
      do 110 jj=1,n
         zs(l)=zs(l)+z(l,jj)*s(l,jj)
110   continue
c
c
      if(np.eq.1) return
c
      do 200 i=2,np
         l=index(i)
         do 210 jj=1,n
            z(l,jj)=diag(jj)*s(l,jj)
210      continue
         do 220 j=1,i-1
            psy=0
            psz=0
            jl=index(j)
            do 230 jj=1,n
               psy=psy+y(jl,jj)*s(l,jj)
               psz=psz+z(jl,jj)*s(l,jj)
230         continue
            do 240 jj=1,n
               z(l,jj)=z(l,jj)+psy*y(jl,jj)/ys(jl)-psz*z(jl,jj)
     &                 /zs(jl)
240         continue
220      continue
c
         zs(l)=0
         do 250 jj=1,n
            zs(l)=zs(l)+z(l,jj)*s(l,jj)
250      continue
200   continue
c
      return
      end
