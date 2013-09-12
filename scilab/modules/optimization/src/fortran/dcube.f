c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine dcube(t,f,fp,ta,fa,fpa,tlower,tupper)
c
c --- arguments
c
      double precision sign,den,anum,t,f,fp,ta,fa,fpa,tlower,tupper
c
c --- variables locales
c
      double precision z1,b,discri
c
c           Using f and fp at t and ta, computes new t by cubic formula
c           safeguarded inside [tlower,tupper].
c
      z1=fp+fpa-3.d0*(fa-f)/(ta-t)
      b=z1+fp
c
c              first compute the discriminant (without overflow)
c
      if (dabs(z1).le.1.d0) then
          discri=z1*z1-fp*fpa
        else
          discri=fp/z1
          discri=discri*fpa
          discri=z1-discri
          if (z1.ge.0.d0 .and. discri.ge.0.d0) then
              discri=dsqrt(z1)*dsqrt(discri)
              go to 120
          endif
          if (z1.le.0.d0 .and. discri.le.0.d0) then
              discri=dsqrt(-z1)*dsqrt(-discri)
              go to 120
          endif
          discri=-1.d0
      endif
      if (discri.lt.0.d0) then
          if (fp.lt.0.d0) t=tupper
          if (fp.ge.0.d0) t=tlower
          go to 900
      endif
c
c  discriminant nonnegative, compute solution (without overflow)
c
      discri=dsqrt(discri)
  120 if (t-ta.lt.0.d0) discri=-discri
      sign=(t-ta)/dabs(t-ta)
      if (b*sign.gt.0.d+0) then
          t=t+fp*(ta-t)/(b+discri)
        else
          den=z1+b+fpa
          anum=b-discri
          if (dabs((t-ta)*anum).lt.(tupper-tlower)*dabs(den)) then
              t=t+anum*(ta-t)/den
            else
              t=tupper
          endif
      endif
  900 t=dmax1(t,tlower)
      t=dmin1(t,tupper)
      return
      end
