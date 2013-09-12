c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine lstops
c ==================================
c     elementary operations on lists
c ==================================
c
c     Copyright INRIA
      include 'stack.h'
c
c
      character tmpbuf * (bsiz)
      logical	ishm
      integer insert,extrac, toto, gettype
      data insert/2/,extrac/3/


c     handle recursion
      if(rstk(pt).ge.403.and.rstk(pt).le.405) goto 11
      if(rstk(pt).eq.406.or.rstk(pt).eq.407) goto 21
c
      if (ddt .eq. 4) then
         write(tmpbuf(1:4),'(i4)') fin
         call basout(io,wte,' lstops '//tmpbuf(1:4))
      endif
c

      if(fin.eq.extrac) goto 10
      if(fin.eq.insert) goto 20
c
c     undefined operations (look for function-defined operation)
      icall = 0
      fun   = 0
      fin   = -fin
      return
c
c     extraction
 10   continue
      toto = gettype(top-1)
      if ( ishm() .and. toto.ne.10 .and. toto.ne.15) then
         call intehm()
         return
      endif

 11   call intl_e()
      return
c
c     insertion
 20   continue
      toto = gettype(top-2) 
      if ( ishm() .and. toto.ne.10 .and. toto.ne.15) then
         call intihm()
         if(err.gt.0) return
         return
      endif
      
 21   call intl_i()
      if(err.gt.0) return
      return
c
      end
