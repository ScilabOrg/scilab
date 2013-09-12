c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine newest(type, uu, vv)
c compute new estimates of quadratic coefficients
c using the scalars computed in calcsc.
      common /gloglo/ p, qp, k, qk, svk, sr, si, u,
     * v, a, b, c, d, a1, a2, a3, a6, a7, e, f, g,
     * h, szr, szi, lzr, lzi, eta, are, mre, n, nn
      double precision p(101), qp(101), k(101),
     * qk(101), svk(101), sr, si, u, v, a ,b,c, d,
     * a1, a2, a3, a6, a7, e, f, g, h, szr, szi,
     * lzr, lzi
      real eta, are, mre
      integer n, nn
      double precision a4, a5, b1, b2, c1, c2, c3,
     * c4, temp, uu, vv
      integer type
c use formulas appropriate to setting of type.
      if (type.eq.3) go to 30
      if (type.eq.2) go to 10
      a4 = a + u*b + h*f
      a5 = c + (u+v*f)*d
      go to 20
   10 a4 = (a+g)*f + h
      a5 = (f+u)*c + v*d
c evaluate new quadratic coefficients.
   20 b1 = -k(n)/p(nn)
      b2 = -(k(n-1)+b1*p(n))/p(nn)
      c1 = v*b2*a1
      c2 = b1*a7
      c3 = b1*b1*a3
      c4 = c1 - c2 - c3
      temp = a5 + b1*a4 - c4
      if (temp.eq.0.0d+0) go to 30
      uu = u - (u*(c3+c2)+v*(b1*a1+b2*a7))/temp
      vv = v*(1.+c4/temp)
      return
c if type=3 the quadratic is zeroed
   30 uu = 0.0d+0
      vv = 0.0d+0
      return
      end
