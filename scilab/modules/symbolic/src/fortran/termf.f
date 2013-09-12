c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine termf(chaine,n,type)
      integer chaine(n),type
c
      integer plus,minus,lparen,rparen
      data plus/45/,minus/46/,lparen/41/,rparen/42/
c
      l1=0
      if(chaine(1).eq.plus.or.chaine(1).eq.minus) l1=l1+1
c
      icount=0
 03   l1=l1+1
      if(l1.gt.n) goto 10
      if(chaine(l1).eq.lparen) then
         icount=icount+1
      else if(chaine(l1).eq.rparen) then
         icount=icount-1
      else
         if(icount.ne.0) goto 03
         if(chaine(l1).eq.plus.or.chaine(l1).eq.minus) goto 20
      endif
      goto 03
c
   10 continue
      type=1
      return
   20 type=0
      return
      end

      subroutine factf(chaine,n,type)
      integer chaine(n),type
c
      integer plus,minus,lparen,rparen,bslash
      data plus/45/,minus/46/,lparen/41/,rparen/42/
      data bslash/49/
c
      l1=0
      if(chaine(1).eq.plus.or.chaine(1).eq.minus) l1=l1+1
c
      icount=0
 03   l1=l1+1
      if(l1.gt.n) goto 10
      if(chaine(l1).eq.lparen) then
         icount=icount+1
      else if(chaine(l1).eq.rparen) then
         icount=icount-1
      else
         if(icount.ne.0) goto 03
         if(chaine(l1).eq.bslash) goto 20
         goto 03
      endif
      goto 03
c
   10 continue
      type=1
      return
   20 type=0
      return
      end
