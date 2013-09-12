c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

C/MEMBR ADD NAME=ISEYE,SSI=0
      logical function iseye(ib)
      integer ib(5),eye(5)
      data eye/14,34,14,41,42/
      iseye=.false.
      do 10 i=1,5
      if (ib(i).ne.eye(i)) return
 10   continue
      iseye=.true.
      return
      end
