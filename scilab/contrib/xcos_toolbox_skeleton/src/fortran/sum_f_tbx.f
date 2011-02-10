c     the inputs are in the form ui,ni, here we have 2 inputs
      subroutine sum_f_tbx(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u1,nu1,u2,nu2,y,ny)

      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*)
      double precision u1(*),u2(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu1,ny

      do 1 i=1,nu1
         y(i)=u1(i)+u2(i)
 1    continue
      return
      end

