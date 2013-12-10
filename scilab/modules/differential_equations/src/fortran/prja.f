C/MEMBR ADD NAME=PRJA,SSI=0
      subroutine prja (neq, y, yh, nyh, ewt, ftem, savf, wm, iwm,
     1   f, jac)
clll. optimize
      
      include 'stack.h'
      
      external f, jac
      integer neq, nyh, iwm
      integer iownd, iowns,
     1   icf, ierpj, iersl, jcur, jstart, kflag, l, meth, miter,
     2   maxord, maxcor, msbp, mxncf, n, nq, nst, nfe, nje, nqu
      integer iownd2, iowns2, jtyp, mused, mxordn, mxords
      integer i, i1, i2, ier, ii, j, j1, jj, lenp,
     1   mba, mband, meb1, meband, ml, ml3, mu
      double precision y, yh, ewt, ftem, savf, wm
      double precision rownd, rowns,
     1   ccmax, el0, h, hmin, hmxi, hu, rc, tn, uround
      double precision rownd2, rowns2, pdnorm
      double precision con, fac, hl0, r, r0, srur, yi, yj, yjj,
     1   vmnorm, fnorm, bnorm
      dimension neq(*), y(*), yh(nyh,*), ewt(*), ftem(*), savf(*),
     1   wm(*), iwm(*)
      common /ls0001/ rownd, rowns(209),
     2   ccmax, el0, h, hmin, hmxi, hu, rc, tn, uround,
     3   iownd(14), iowns(6),
     4   icf, ierpj, iersl, jcur, jstart, kflag, l, meth, miter,
     5   maxord, maxcor, msbp, mxncf, n, nq, nst, nfe, nje, nqu
      common /lsa001/ rownd2, rowns2(20), pdnorm,
     1   iownd2(3), iowns2(2), jtyp, mused, mxordn, mxords
c-----------------------------------------------------------------------
c%purpose
c prja is called by stoda to compute and process the matrix
c p = i - h*el(1)*j , where j is an approximation to the jacobian.
c here j is computed by the user-supplied routine jac if
c miter = 1 or 4 or by finite differencing if miter = 2 or 5.
c j, scaled by -h*el(1), is stored in wm.  then the norm of j (the
c matrix norm consistent with the weighted max-norm on vectors given
c by vmnorm) is computed, and j is overwritten by p.  p is then
c subjected to lu decomposition in preparation for later solution
c of linear systems with p as coefficient matrix. this is done
c by dgefa if miter = 1 or 2, and by dgbfa if miter = 4 or 5.
c
c%additional  parameters
c in addition to variables described previously, communication
c with prja uses the following..
c y     = array containing predicted values on entry.
c ftem  = work array of length n (acor in stoda).
c savf  = array containing f evaluated at predicted y.
c wm    = real work space for matrices.  on output it contains the
c         lu decomposition of p.
c         storage of matrix elements starts at wm(3).
c         wm also contains the following matrix-related data..
c         wm(1) = sqrt(uround), used in numerical jacobian increments.
c iwm   = integer work space containing pivot information, starting at
c         iwm(21).   iwm also contains the band parameters
c         ml = iwm(1) and mu = iwm(2) if miter is 4 or 5.
c el0   = el(1) (input).
c pdnorm= norm of jacobian matrix. (output).
c ierpj = output error flag,  = 0 if no trouble, .gt. 0 if
c         p matrix found to be singular.
c jcur  = output flag = 1 to indicate that the jacobian matrix
c         (or approximation) is now current.
c this routine also uses the common variables el0, h, tn, uround,
c miter, n, nfe, and nje.
c!
c-----------------------------------------------------------------------
      nje = nje + 1
      ierpj = 0
      jcur = 1
      hl0 = h*el0
      go to (100, 200, 300, 400, 500), miter
c if miter = 1, call jac and multiply by scalar. -----------------------
 100  lenp = n*n
      do 110 i = 1,lenp
 110    wm(i+2) = 0.0d+0
      call jac (neq, tn, y, 0, 0, wm(3), n)
      if(ierror.gt.0) return
      con = -hl0
      do 120 i = 1,lenp
 120    wm(i+2) = wm(i+2)*con
      go to 240
c if miter = 2, make n calls to f to approximate j. --------------------
 200  fac = vmnorm (n, savf, ewt)
      r0 = 1000.0d+0*abs(h)*uround*dble(n)*fac
      if (r0 .eq. 0.0d+0) r0 = 1.0d+0
      srur = wm(1)
      j1 = 2
      do 230 j = 1,n
        yj = y(j)
        r = max(srur*abs(yj),r0/ewt(j))
        y(j) = y(j) + r
        fac = -hl0/r
        call f (neq, tn, y, ftem)
      if(ierror.gt.0) return
        do 220 i = 1,n
 220      wm(i+j1) = (ftem(i) - savf(i))*fac
        y(j) = yj
        j1 = j1 + n
 230    continue
      nfe = nfe + n
 240  continue
c compute norm of jacobian. --------------------------------------------
      pdnorm = fnorm (n, wm(3), ewt)/abs(hl0)
c add identity matrix. -------------------------------------------------
      j = 3
      do 250 i = 1,n
        wm(j) = wm(j) + 1.0d+0
 250    j = j + (n + 1)
c do lu decomposition on p. --------------------------------------------
      call dgefa (wm(3), n, n, iwm(21), ier)
      if (ier .ne. 0) ierpj = 1
      return
c dummy block only, since miter is never 3 in this routine. ------------
 300  return
c if miter = 4, call jac and multiply by scalar. -----------------------
 400  ml = iwm(1)
      mu = iwm(2)
      ml3 = ml + 3
      mband = ml + mu + 1
      meband = mband + ml
      lenp = meband*n
      do 410 i = 1,lenp
 410    wm(i+2) = 0.0d+0
      call jac (neq, tn, y, ml, mu, wm(ml3), meband)
      if(ierror.gt.0) return
      con = -hl0
      do 420 i = 1,lenp
 420    wm(i+2) = wm(i+2)*con
      go to 570
c if miter = 5, make mband calls to f to approximate j. ----------------
 500  ml = iwm(1)
      mu = iwm(2)
      mband = ml + mu + 1
      mba = min(mband,n)
      meband = mband + ml
      meb1 = meband - 1
      srur = wm(1)
      fac = vmnorm (n, savf, ewt)
      r0 = 1000.0d+0*abs(h)*uround*dble(n)*fac
      if (r0 .eq. 0.0d+0) r0 = 1.0d+0
      do 560 j = 1,mba
        do 530 i = j,n,mband
          yi = y(i)
          r = max(srur*abs(yi),r0/ewt(i))
 530      y(i) = y(i) + r
        call f (neq, tn, y, ftem)
      if(ierror.gt.0) return
        do 550 jj = j,n,mband
          y(jj) = yh(jj,1)
          yjj = y(jj)
          r = max(srur*abs(yjj),r0/ewt(jj))
          fac = -hl0/r
          i1 = max(jj-mu,1)
          i2 = min(jj+ml,n)
          ii = jj*meb1 - ml + 2
          do 540 i = i1,i2
 540        wm(ii+i) = (ftem(i) - savf(i))*fac
 550      continue
 560    continue
      nfe = nfe + mba
 570  continue
c compute norm of jacobian. --------------------------------------------
      pdnorm = bnorm (n, wm(3), meband, ml, mu, ewt)/abs(hl0)
c add identity matrix. -------------------------------------------------
      ii = mband + 2
      do 580 i = 1,n
        wm(ii) = wm(ii) + 1.0d+0
 580    ii = ii + meband
c do lu decomposition of p. --------------------------------------------
      call dgbfa (wm(3), meband, n, ml, mu, iwm(21), ier)
      if (ier .ne. 0) ierpj = 1
      return
c----------------------- end of subroutine prja ------------------------
      end
