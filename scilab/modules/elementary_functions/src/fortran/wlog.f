c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) Bruno Pincon
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine wlog(xr,xi,yr,yi)
*
*     PURPOSE
*        wlog compute the logarithm of a complex number
*        y = yr + i yi = log(x), x = xr + i xi 
*
*     CALLING LIST / PARAMETERS
*        subroutine wlog(xr,xi,yr,yi)
*        double precision xr,xi,yr,yi
*
*        xr,xi: real and imaginary parts of the complex number
*        yr,yi: real and imaginary parts of the result
*               yr,yi may have the same memory cases than xr et xi
*         
*     METHOD 
*        adapted with some modifications from Hull, 
*        Fairgrieve, Tang, "Implementing Complex 
*        Elementary Functions Using Exception Handling", 
*        ACM TOMS, Vol. 20 (1994), pp 215-244
*
*        y = yr + i yi = log(x)
*        yr = log(|x|) = various formulae depending where x is ...
*        yi = Arg(x) = atan2(xi, xr)
*        
*     AUTHOR
*        Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
*
      implicit none

*     PARAMETER
      double precision xr, xi, yr, yi

*     LOCAL VAR
      double precision a, b, t, r
*     CONSTANTS
      double precision R2
      parameter (R2 =  1.41421356237309504d0)

*     EXTERNAL
      double precision dlamch, logp1, pythag
      external         dlamch, logp1, pythag


*     STATIC VAR
      logical first
	double precision RMAX, LSUP, LINF

      save    first
      data    first /.true./
           save             RMAX, LSUP, LINF
      
      if (first) then
         RMAX = dlamch('O')
         LINF = sqrt(dlamch('U'))
         LSUP = sqrt(0.5d0*RMAX)
         first = .false.
      endif

*     (0) avoid memory pb ...
      a = xr
      b = xi

*     (1) compute the imaginary part
      yi = atan2(b, a)

*     (2) compute the real part
      a = abs(a)
      b = abs(b)

*     Order a and b such that 0 <= b <= a
      if (b .gt. a) then
         t = b
         b = a
         a = t
      endif

      if ( (0.5d0 .le. a) .and. (a .le. R2) ) then
         yr = 0.5d0*logp1((a-1.d0)*(a+1.d0) + b*b)
      elseif (LINF .lt. b .and. a .lt. LSUP) then
*        no overflow or underflow can occur in computing a*a + b*b 
         yr = 0.5d0*log(a*a + b*b)
      elseif (a .gt. RMAX) then
*        overflow
         yr = a
      else
         t = pythag(a,b)
         if (t .le. RMAX) then
            yr = log(t)
         else
*           handle rare spurious overflow with :
            r = b/a
            yr = log(a) + 0.5d0*logp1(r*r)
         endif
      endif

      end

