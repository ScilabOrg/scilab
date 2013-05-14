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
      subroutine intg
c     --------------------------------------------
c     Scilab intg
c      implicit undefined (a-z)
      character*(4) fname
      character*6   namef
      include 'stack.h'
      integer iero
      common/ierajf/iero
      common/cintg/namef
      external bintg,fintg
      double precision epsa,epsr,a,b,val,abserr
      logical getexternal, getscalar,type ,cremat
      integer topk,lr,katop,kydot,top2,lra,lrb,lc
      integer iipal,lpal,lw,liw,lpali,ifail
      integer iadr,sadr,vfinite
      external setfintg
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      fname='intg'
      if(rhs.lt.3) then
         call error(39)
         return
      endif
      type=.false.
      top2=top
      topk=top
      if(rhs.eq.5) then
         if (.not.getscalar(fname,topk,top,lr)) return
         epsr=stk(lr)
         top=top-1
      else
         epsr=1.0d-8
      endif
      if (rhs.ge.4) then
         if (.not.getscalar(fname,topk,top,lr)) return
         epsa=stk(lr)
         top=top-1
      else
         epsa=1.0d-14
      endif
c     cas standard
      if (.not.getexternal(fname,topk,top,namef,type,
     $     setfintg)) return
      kydot=top
      top=top-1
      if (.not.getscalar(fname,topk,top,lrb)) return
      b=stk(lrb)
      if (isanan(b).eq.1.or.vfinite(1,b).eq.0) then
         err = 2
         call error(264)
         return
      endif
      top=top-1
      katop=top
      if (.not.getscalar(fname,topk,top,lra)) return
      a=stk(lra)
      if (isanan(a).eq.1.or.vfinite(1,a).eq.0) then
         err = 1
         call error(264)
         return
      endif
c     tableaux de travail
      top=top2+1
      lw=3000
      if (.not.cremat(fname,top,0,1,lw,lpal,lc)) return
      top=top+1
c     tableau de travail entier necessaire
      liw=3000/8+2
      if (.not.cremat(fname,top,0,1,iadr(liw)+1,lpali,lc)) return
      top=top+1
c
c     external scilab
c
      iipal=iadr(lstk(top))
      istk(iipal)=1
      istk(iipal+1)=iipal+2
      istk(iipal+2)=kydot
      istk(iipal+3)=katop
      lstk(top+1)=sadr(iipal+4)
      if(type) then
         call dqags(fintg,a,b,epsa,epsr,val,abserr,
     +        neval,ifail,lw/4,lw,last,stk(lpali),stk(lpal))
      else
         call dqags(bintg,a,b,epsa,epsr,val,abserr,
     +        neval,ifail,lw/4,lw,last,stk(lpali),stk(lpal))
      endif
      if(err.gt.0.or.err1.gt.0) return
      if(ifail.gt.0) then
         select case (ifail)
            case (1)
               call erro('Error: Maximum number of subdivisons '//
     &            'achieved. Splitting the interval might help.')
               return
            case (2)
               call erro('Error: Round-off error detected, the '//
     &         'requested tolerance (or default) cannot be achieved.'//
     &         ' Try using bigger tolerances.')
               return
            case (3)
               call erro('Error: Bad integrand behavior occurs at '//
     &            'some points of the integration interval.')
               return
            case (4)
               call erro('Error: Convergence problem, round-off '//
     &            'error detected. Try using bigger tolerances.')
               return
            case (5)
               call erro('Error: The integral is probably '//
     &            'divergent, or slowly convergent.')
               return
c           case (6)
            case default
               call erro('Error: Invalid input, absolute '//
     &            'tolerance <= 0 and relative tolerance < 2.e-14.')
               return
         end select
      endif
      top=top2-rhs+1
      stk(lra)=val
      if(lhs.eq.2) then
         top=top+1
         stk(lrb)=abserr
         return
      endif
      return
      end

