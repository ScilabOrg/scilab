c     --------------------------
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

c     --------------------------
      subroutine scigschur(fname)
c     Copyright INRIA
      INCLUDE 'stack.h'
c     
      character*(*) fname
      
c     obsolete 
c     call macro for emulation
      call cvname(ids(1,pt+1),'gschur',0)
      fun=-1
      return
      end
c     --------------------------
      
      