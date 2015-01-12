c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine write_double(form, dat, m, n)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      double precision dat(*)
      integer m,n
      character form*(*)
      
      !write (*, '(f)') dat(1)
      !write (*, '(f)') dat(2)
      do 65 i=1,m
          write(buf,form) (dat(j*m+i),j=0, n-1)
          lb1=lch
 66       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 66
      
          io = 0
          call basout(io, 6, buf(1:lb1))
 65   continue      
      
      end

      subroutine write_int(form, dat, m, n)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      integer dat(*)
      integer m,n
      character form*(*)
      
      !write (*, '(f)') dat(1)
      !write (*, '(f)') dat(2)
      do 67 i=1,m
          write(buf,form) (dat(j*m+i),j=0, n-1)
          lb1=lch
 68       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 68
      
          io = 0
          call basout(io, 6, buf(1:lb1))
 67   continue      
      
      end      


      subroutine write_string(form, dat)

      parameter (lch=4096)
      character buf*(lch)
      character form*(*)
      character dat*(*)
      
      write(buf,form) dat
      lb1=lch
 69   lb1=lb1-1
      if(buf(lb1:lb1).eq.' ') goto 69
      
      call basout(io, 6, buf)
  
      end

      subroutine write_string_file(ID, form, dat)
      
      parameter (lch=4096)
      character buf*(lch)
      integer ID
      character form*(*)
      character dat*(*)
      
      do 99 i=1,len(dat),lch
          if(len(dat) < (i+lch-1)) then
              write(ID,form) dat(i:len(dat))
          else          
              write(ID,form) dat(i:(i+lch-1))
          end if
          
99    continue
      end  
      
      subroutine write_double_file(ID, form, dat,  m, n)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      double precision dat(*)
      integer m,n
      character form*(*)
      
      !write (*, '(f)') dat(1)
      !write (*, '(f)') dat(2)
      do 71 i=1,m
          write(ID,form) (dat(j*m+i),j=0, n-1)
      
 71   continue      
      
      end

      subroutine write_int_file(ID, form, dat, m, n)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      integer dat(*)
      integer m,n
      character form*(*)
      
      !write (*, '(f)') dat(1)
      !write (*, '(f)') dat(2)
      do 72 i=1,m
          write(ID,form) (dat(j*m+i),j=0, n-1)
      
 72   continue      
      
      end 

      subroutine write_double_szsc(form, dat, szsc, m, n)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      character buf2*(lch)
      double precision dat(*)
      integer m,n,szsc
      character form*(*)
      
      do 73 i=1,m
          write(buf,form) (dat(j*m+i),j=0, n-1)
          lb1=lch
 74       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 74
      
      do 75 j=1,lb1,szsc
          if(lb1 < (j+szsr-1)) then
              write(buf2,*) buf(j:lb1)
              
          else          
              write(buf2,*) buf(j:(j+szsc-1))
          end if
          
        io = 0
        call basout(io, 6, buf2)
 75   continue   
 73   continue      
      
      end

      subroutine write_double_szsc_file(ID, form, dat, szsc,  m, n)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      double precision dat(*)
      integer m,n,szsc
      character form*(*)
      
      do 76 i=1,m
          write(buf,form) (dat(j*m+i),j=0, n-1)
          lb1=lch
 77       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 77
      
      do 78 j=1,lb1,szsc
          if(lb1 < (j+szsc-1)) then
              write(ID,*) buf(j:lb1)
              
          else          
              write(ID,*) buf(j:(j+szsc-1))
          end if

 78   continue   
 76   continue  
      
      end

      subroutine write_int_szsc(form, dat, szsc, m, n)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      character buf2*(lch)
      integer dat(*)
      integer m,n,szsc
      character form*(*)
      
      do 79 i=1,m
          write(buf,form) (dat(j*m+i),j=0, n-1)
          lb1=lch
 80       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 80
      
      do 81 j=1,lb1,szsc
          if(lb1 < (j+szsr-1)) then
              write(buf2,*) buf(j:lb1)
              
          else          
              write(buf2,*) buf(j:(j+szsc-1))
          end if
          
        io = 0
        call basout(io, 6, buf2)
 81   continue   
 79   continue      
      
      end

      subroutine write_int_szsc_file(ID, form, dat, szsc,  m, n)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      integer dat(*)
      integer m,n,szsc
      character form*(*)
      
      do 82 i=1,m
          write(buf,form) (dat(j*m+i),j=0, n-1)
          lb1=lch
 83       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 83
      
      do 84 j=1,lb1,szsc
          if(lb1 < (j+szsc-1)) then
              write(ID,*) buf(j:lb1)
              
          else          
              write(ID,*) buf(j:(j+szsc-1))
          end if

 84   continue   
 82   continue  
      
      end