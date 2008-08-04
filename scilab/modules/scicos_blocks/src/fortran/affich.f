      subroutine affich(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Displays the value of the input in a graphic window
c
c     ipar(1) = font
c     ipar(2) = fontsize
c     ipar(3) = color
c     ipar(4) = win
c     ipar(5) = nt : total number of output digits
c     ipar(6) = nd number of rationnal part digits

c
c     z(1)=value
c     z(2)=window
c     z(3)=x
c     z(4)=y
c     z(5)=width
c     z(6)=height
c     z(7)=text_handle

      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      double precision g(6)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny


      integer wid

      double precision sciround,ur


c     
c     
      if(flag.eq.2) then
c     state evolution
         ur=10.0d0**ipar(6)
         ur=sciround(u(1)*ur)/ur
         if (ur.eq.z(1)) return
         wid=z(2)
         if(wid.lt.0) return

         call setblockwin(int(z(2)),cur)


         z(1)=ur

         
         call affdraw(ipar(5),z(1), z(2), z(7))

c     INIT ----------------------------------------------------------
      elseif(flag.eq.4) then
c     init
c     .  initial value = 0         
         z(1)=0.0d0 
c     .  get geometry of the block
         call getgeom(z(2))
      
         
c     . Check that the winId is valid.
         if(z(2).lt.0.0d0) return
         
         
      call initdraw(ipar(1), ipar(2), ipar(3), z(2), z(3), z(4), z(5),
     &              z(6), z(7))

      endif
      end

      subroutine setblockwin(win,cur)
      integer win,cur
      integer v,verb
      double precision dv
      data verb/0/

      return
      end

c --------------------------------------------------
      subroutine affdraw(form, val, winnum, textind)
      integer textind 
      integer form(2)
      double precision g(6)
      double precision winnum
      double precision val
      character*40 fmt,value

c     . Compute the string to display
      write(fmt,'(''(f'',i3,''.'',i3,'')'')') form(1),form(2)
      value=' '
      write(value,fmt) val
      ln=lnblnk(value)
      value(ln+1:ln+1)=char(0)
       
       call affichup(textind, winnum, value)
       
 
      return
 
      end


c -------------------------------------------------------------------------------------


      subroutine getgeom(g)
      include "stack.h"
      double precision g(*)
      integer scicstring,ret
      integer sadr,iadr
      iadr(l) = l + l - 1
      sadr(l)=(l/2)+1
      ret=scistring(rhs+1, 'getgeom',1,0)
      il=iadr(lstk(top+1))
      l=sadr(il+4)
c     . 6 : number of element to retrieve
      call dcopy(6,stk(l),1,g,1)
c      top=top-1
      return
      end
c --------------------------------------------------
      subroutine initdraw(fontid, fontsize, color, winnum, x, y,
     &                    width, height, textind)
      integer fontId
      integer fontSize
      integer color
      double precision winNum
      double precision x
      double precision y
      double precision width
      double precision height

      call affichin(fontid, fontsize, color, winnum, x, y, width,
     &              height, textind)
 
      return
 
      end


c -------------------------------------------------------------------------------------
