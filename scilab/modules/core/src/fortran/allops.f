c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine allops
c ======================================================================
c     Calling function according to arguments type
c ======================================================================
      include 'stack.h'
      integer ogettype, vt,vt1,id(nsiz),r,op,bl(nsiz)
      logical compil,ptover
      integer iadr
      character tmpbuf * (bsiz)

c
      iadr(l)=l+l-1

      r=0
      if(pt.gt.0) r=rstk(pt)
c
      if (ddt .eq. 4) then
         write(tmpbuf(1:12),'(3i4)') fin,pt,r
         call basout(io,wte,' allops op:'//tmpbuf(1:4)//' pt:'
     &    //tmpbuf(5:8)//
     &    ' rstk(pt):'//tmpbuf(9:12))
      endif
c
c     compilation allops :<5 fin rhs lhs>
      if ( compil(5,fin,rhs,lhs,0)) then
         if (err.gt.0) return
         fun=0
         return
      endif
c
 01   ir=r/100
      if(ir.eq.4) then
         if (r.eq.401) then
            pt=pt-1
            call putid(syn(1),ids(1,pt))
            pt=pt-1
         elseif (r.eq.402) then
            pt=pt-1
         elseif (r.ge.403.and.r.le.407) then
            goto 51
         elseif (r.eq.408) then
            goto 60
         endif
         return
      endif
      if(err1.gt.0) return
 02   vt=0


      if(fin.eq.2) then
c     . insertions
         icall=0
         nt=2
         vt1=abs(ogettype(top))
         if(vt1.eq.15.or.vt1.eq.16.or.vt1.eq.17) then
c     .     every thing can be inserted in a list
            goto 50
         endif
      elseif(fin.eq.3) then
c     .  extraction
         if(icall.ne.4) then
            if(rhs.le.1) then
c     .     a() -->a
               if (rhs.eq.0) rhs=rhs+1
               call ref2val
               goto 81
            endif
            nt=1
            icall=0
         else
            icall=0
            vt1=abs(ogettype(top))
            if (vt1.eq.11.or.vt1.eq.13) then
c     .        extraction reveals to be  function execution
               il=iadr(lstk(top))
               fin=istk(il+1)
               top=top-1
               rhs=rhs-1
               if (ptover(1,psiz)) return
               call putid(ids(1,pt),bl)
               rstk(pt)=0
               if (ptover(1,psiz)) return
               call putid(ids(1,pt),bl)
               rstk(pt)=401

               icall=5
c     .  *call* macro
               return
            elseif (vt1.eq.130) then
c     .        extraction reveals to be primitive function execution
               il=iadr(lstk(top))
               il=iadr(istk(il+1))
               fun=istk(il+1)
               fin=istk(il+2)
               top=top-1
               rhs=rhs-1
               if (ptover(1,psiz)) return
               rstk(pt)=402
               icall=9
c     .  *call* matfns
               return
            else
               if(rhs.eq.1) then
c     .     a() -->a
                  goto 81
               endif
               nt=1
            endif
         endif
      else
         call ref2val
         nt=rhs
      endif
      do 03 i=1,nt
         vt1=abs(ogettype(top+1-i))
         if(vt1.eq.0) then
            err=nt
            call error(44)
            return
         endif
         if(vt1.gt.vt) vt=vt1
 03   continue

c
 04   goto (10,20,06,30,70,35,05,75,76,40,60,05,60,60,50,50,50) ,vt
c     overloadable ops
      if(vt.eq.129.and.fin.eq.3) goto 20
 05   op=fin
      goto 90

 06   call error(43)
      return
 10   call matops
      goto 80
 20   call polops
      goto 80
 30   call logic
      goto 80
 35   call lspops
      goto 80
 40   call strops
      goto 80
 50   continue
c     change rstk(pt) if necessary to avoid bad interpretation in intl_i
      if(rstk(pt).eq.406) rstk(pt)=409
 51   call lstops
      if(err.gt.0.or.err1.gt.0) return
      if(icall.eq.4) goto 02
      goto 81
 60   call misops
      goto 80
 70   call spops
      goto 80
 75   call intops
      goto 80
 76   call hndlops
      goto 80

c
 80   if(err.gt.0.or.err1.gt.0) return
 81   call iset(rhs,0,infstk(max(top-lhs+1,1)),1)
      if(rstk(pt).eq.409)  rstk(pt)=406
c
      if(fun.ne.0) then
c     .  appel d'un matfn necessaire pour achever l'evaluation
         if (ptover(1,psiz)) return
         rstk(pt)=402
         icall=9
c     .  *call* matfns
         return
      endif
c
      if(icall.eq.5) return

      if(fin.lt.0) then
         op=-fin
         fin=-fin
         goto 90
      endif

c      if(rstk(pt).eq.406.or.rstk(pt).eq.405) then
      if(rstk(pt).eq.406) then
c     .  list recursive extraction insertion
         goto 51
      endif
      return

 90   continue
c     .  operation macro programmee ?
      call mname(op,id)
      if(err.gt.0.or.err1.gt.0) return

      if(fun.gt.0) then
         if (ptover(1,psiz)) return
         rstk(pt)=402
         icall=9
c     .  *call* matfns
         return
      else
c         call ref2val
         fin=lstk(fin)
         if (ptover(1,psiz)) return
         call putid(ids(1,pt),syn)
         rstk(pt)=0
         if (ptover(1,psiz)) return
         call putid(ids(1,pt),id)
         rstk(pt)=401
         icall=5
c     .  *call* macro
         return
      endif
c
      end
