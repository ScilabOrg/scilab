c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine write_double(form, dat, m, n)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      double precision dat(*)
      integer m,n
      character form*(*)
      
      !write (*, '(f)') dat(1)
      !write (*, '(f)') dat(2)
      do 65 i=1,m
          write(buf,form) (dat(j*m+i),j=0, n-1)
          lb1=lch
 66       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 66
      
          io = 0
          call basout(io, 6, buf(1:lb1))
 65   continue      
      
      end      
