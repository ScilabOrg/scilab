c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intdisp

      INCLUDE 'stack.h'
      integer topk,id(nlgh)
      integer iadr
      logical eptover
c
      iadr(l)=l+l-1
c

      if(rstk(pt).eq.908 ) goto 203

      if(rhs.lt.1) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif

      il=iadr(lstk(top))

c     loop on variable to display
      i=1
 201  continue
      topk=top
      id(1)=0
 202  call print(id,topk,wte)
      if(topk.eq.0) goto 204
c     overloaded display 
      if ( eptover(1,psiz)) return
      rstk(pt)=908
      pstk(pt)=i
      ids(1,pt)=rhs
      if(fun.eq.0) then
c     *call* macro
         icall=5
      else
c     *call* matfns
      endif
      return
 203  continue
      i=pstk(pt)
      rhs=ids(1,pt)
      pt=pt-1
      goto 202
 204  continue
c     next variable to display
      i=i+1
      top=top-1
      if(i.le.rhs) goto 201

      top=top+1
      call objvide('disp',top)
 999  return
      end
