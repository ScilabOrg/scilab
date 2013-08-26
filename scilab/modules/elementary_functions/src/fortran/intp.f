c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine intp(x,xd,yd,n,nc,y)
c!purpose
c     linear interpolation computes y=F(x) for f a tabulated function
c      from R to R^n 
c!parameters
c     x    : x given point
c     xd   : vector (nc) of abscissae mesh points (xd(i+1)>=xd(i))
c     yd   : matrix (nc x n): yd(i,j)=Fj(x(i))
c     n    : dimension of F image
c     returned values
c     y    : vector (n) :interpolated value of F(x)
c!remarks
c     if x<=xd(1) y=yd(1,:)
c      if x>=xd(nc) y=yd(nc,:)
c!origin
c     Pejman GOHARI 1996
c
      double precision x,xd(*),y(*),yd(nc,*)
      integer n,nc
c     
      if (nc.eq.1) then
         call dcopy(n,yd(1,1),nc,y,1)
      elseif(x.ge.xd(nc)) then 
         call dcopy(n,yd(nc,1),nc,y,1)
      elseif(x.le.xd(1)) then 
         call dcopy(n,yd(1,1),nc,y,1)
      else
c     find x interval
         do 10 i=1,nc
            if (x.lt.xd(i)) then
               inter=i-1
               goto 20
            endif
 10      continue
 20      continue
c     
c     compute interpolated y
c     
         if (xd(inter+1).eq.xd(inter)) then
            call dcopy(n,yd(inter,1),nc,y,1)
         else
            do 40 i=1,n
               y(i)=yd(inter,i)+
     &              (x-xd(inter))*((yd(inter+1,i)-yd(inter,i))/
     &              (xd(inter+1)-xd(inter)))
 40         continue
         endif
      endif
      end
      
