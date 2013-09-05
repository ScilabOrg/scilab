c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine read_inter(lunit,fmt)
c     interface for "file" gateway
        integer lunit
        character*(*) fmt
c
        read(lunit, fmt, err=20, end=30)
c
        return 0
   20   return 2
   30   return 1
      end
