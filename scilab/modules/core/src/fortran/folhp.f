c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      integer function folhp(ls,alpha,beta,s,p)
      integer ls
      double precision alpha,beta,s,p
c!purpose
c      this routine checks if
c    the real root alpha/beta lies in the open left half plane
c       (if ls=1)
c     the complex conjugate roots with sum s and product p lie
c     in the open left half plane (if ls=2)
c     if so, folhp=1, otherwise, folhp=-1
c      in this function the parameter p is not referenced
c
c!calling sequence
c
c     integer function folhp(ls,alpha,beta,s,p)
c     integer ls
c     double precision alpha,beta,s,p
c!auxiliary routines
c     none
c!
      folhp=-1
      if(ls.eq.2) go to 2
      if(alpha*beta.lt.0.0d+0) folhp=1
      return
 2    if(s.lt.0.0d+0) folhp=1
      return
      end
