c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine wasin(zr, zi, ar, ai)
*
*     PURPOSE
*        Compute the arcsin of a complex number
*         a = ar + i ai = asin(z), z = zr + i zi
*       
*     CALLING LIST / PARAMETERS
*        subroutine wasin(zr,zi,ar,ai)
*        double precision zr,zi,ar,ai
*
*        zr,zi: real and imaginary parts of the complex number
*        ar,ai: real and imaginary parts of the result
*               ar,ai may have the same memory cases than zr et zi
*
*     REFERENCE
*        This is a Fortran-77 translation of an algorithm by 
*        T.E. Hull, T. F. Fairgrieve and P.T.P. Tang which 
*        appears in their article :
*          "Implementing the Complex Arcsine and Arccosine 
*           Functions Using Exception Handling", ACM, TOMS, 
*           Vol 23, No. 3, Sept 1997, p. 299-335
*
*        with some modifications so as don't rely on ieee handle
*        trap functions.
*
*     AUTHOR
*        Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
*        Thanks to Tom Fairgrieve
*
      implicit none

*     PARAMETERS
      double precision zr, zi, ar, ai

*     EXTERNAL FUNCTIONS
      double precision dlamch, logp1
      external         dlamch, logp1
      integer          isanan
      external         isanan

*     CONSTANTS
      double precision LN2, HALFPI, Across, Bcross
      parameter       (LN2    = 0.6931471805599453094172321d0,
     $                 HALFPI = 1.5707963267948966192313216d0,
     $                 Across = 1.5d0,
     $                 Bcross = 0.6417d0)  

*     LOCAL VARIABLES
      double precision x, y, A, B, R, S, Am1, szr, szi


*     STATIC VARIABLES
      double precision LSUP, LINF, EPSM
      save             LSUP, LINF, EPSM
      logical          first
      save             first
      data             first /.true./
*     TEXT
*     got f.p. parameters used by the algorithm
      if (first) then
          LSUP = sqrt(dlamch('o'))/8.d0
          LINF = 4.d0*sqrt(dlamch('u'))
          EPSM = sqrt(dlamch('e'))
          first = .false.
      endif

*     avoid memory pb ...
      x = abs(zr)
      y = abs(zi)
      szr = sign(1.d0,zr)
      szi = sign(1.d0,zi)


      if (LINF .le. min(x,y) .and. max(x,y) .le. LSUP ) then
*        we are in the safe region
         R = sqrt((x+1.d0)**2 + y**2)
         S = sqrt((x-1.d0)**2 + y**2)
         A = 0.5d0*(R + S)
         B = x/A

*        compute the real part
         if ( B .le. Bcross ) then
            ar = asin(B)
         elseif ( x .le. 1.d0 ) then
            ar = atan( x / sqrt( 
     $              0.5d0*(A+x)*((y**2)/(R+(x+1.d0))+(S+(1.d0-x)))) )
         else
            ar = atan( x / 
     $          (y*sqrt(0.5d0*((A+x)/(R+(x+1.d0))+(A+x)/(S+(x-1.d0))))))
         endif

*        compute the imaginary part
         if ( A .le. Across ) then
            if ( x .lt. 1.d0 ) then
               Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(y**2)/(S+(1.d0-x)))
            else
               Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(S+(x-1.d0)))
            endif
            ai = logp1(Am1 + sqrt(Am1*(A+1.d0)))
         else
            ai = log(A + sqrt(A**2 - 1.d0))
         endif

      else
*        HANDLE BLOC : evaluation in the special regions ...
         if ( y .le. EPSM*abs(x-1.d0) ) then
            if (x .lt. 1.d0 ) then
               ar = asin(x)
               ai = y/sqrt((1.d0+x)*(1.d0-x))
            else
               ar = HALFPI
               if ( x .le. LSUP ) then
                  ai = logp1((x-1.d0) + sqrt((x-1.d0)*(x+1.d0)))
               else
                  ai = LN2 + log(x)
               endif
            endif

         elseif (y .lt. LINF) then
            if (isanan(x).eq.1) then
               ar = x
            else
               ar = HALFPI - sqrt(y)
            endif
            ai = sqrt(y)

         elseif (EPSM*y - 1.d0 .ge. x) then
            ar = x/y
            ai = LN2 + log(y)

         elseif (x .gt. 1.d0) then
            ar = atan(x/y)
            ai = LN2 + log(y) + 0.5d0*logp1((x/y)**2)

         else
            A = sqrt(1 + y**2)
            ar = x/A
            ai = 0.5*logp1(2.d0*y*(y+A))
         endif
      endif

*     recover the signs
      ar = szr * ar
      if (y.eq.0d00 .and. szr.gt.0d00) then
          szi = - szi
      endif
      ai = szi * ai

      end


