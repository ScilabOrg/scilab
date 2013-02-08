c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
c     Examples for daskr 
c     ----------------------
c
c
      subroutine psol1(neq, t, y, ydot, savr, wk, cj, wght,
     *                   wp, iwp, b, eplin, ier, rpar, ipar)
      double precision t, y(*), ydot(*), savr(*), wk(*), cj, wght(*),
     *                  wp(*), b(*), eplin, rpar(*)
      integer neq, iwp(*), ier, ipar(*)
      print *,"inpsol"
      ier = -1
      ib = 1
      ibd = 1
      do 20 jy = 1,neq
        do 10 jx = 1,neq
c          call DGESL (wp(ibd), neq, neq, iwp(ib), b(ib), 0)
          ib = ib + neq
          IBD = ibd + neq**2
 10       continue
 20     continue
c      b = P\b
      ier = 0
      print *,"endpsol"
      return
      end

