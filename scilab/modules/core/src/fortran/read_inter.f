c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine read_double_file(ID, dat, m, n, ierr)

      double precision dat(*)
      integer m,n
      integer ierr
      
      do 70 i=1,m
          read(ID,*,end=10,err=20) (dat(j*m+i),j=0, n-1)
      
 70   continue 
      return

10      ierr = 1
      return        
20      ierr = 2
      return 
      
      end

      subroutine read_double_file_form(ID, form, dat,  m, n, ierr)

      double precision dat(*)
      integer m,n
      character form*(*)
      integer ierr
      
      do 71 i=1,m
          read(ID,form,end=10,err=20) (dat(j*m+i),j=0, n-1)
      
 71   continue    
      return
      
10      ierr = 1
      return        
20      ierr = 2
      return 
      
      end

      subroutine read_double_line_file(ID, dat, n, ierr)

      double precision dat(*)
      integer n
      integer m
      integer ierr

      read(ID,*,end=10,err=20) dat(1:n)
      
      return

10      ierr = 1
      return        
20      ierr = 2
      return 
      
      end
      
      subroutine read_double_line_file_form(ID, form, dat, n, ierr)

      double precision dat(*)
      integer n
      character form*(*)
      integer ierr

      read(ID,form,end=10,err=20) dat(1:n)
      
      return

10      ierr = 1
      return        
20      ierr = 2
      return 
      
      end
      
       subroutine read_string_file(ID, form, dat, siz, ierr)

      parameter (lch=4096)
      character dat*(lch)
      character form*(*)
      integer siz,ierr
                     
      read(ID,form,end=10,err=20) dat
      mn = lch;
127   mn=mn-1
      if(dat(mn:mn).eq.' ') goto 127
            siz=max(1,mn)
      return

10      ierr = 1
      return        
20      ierr = 2
      return  
            
         
      end

      subroutine read_string(form, dat, siz, ierr)

      parameter (lch=4096)
      character string*(lch)
      character dat*(lch)
      
      character form*(*)
      integer siz,ierr
             
      string=' '
      call eventloopprompt(string,len(string),lline,status,0,iflag)
      if(status.ne.0) goto 10
      if (lline.eq.0) then
        string(1:1)=' '
        lline=1
      endif
      
      read(string,form,end=10,err=20) dat
      mn = lch;
128   mn=mn-1
      if(dat(mn:mn).eq.' ') goto 128
            siz=max(1,mn)
      return

10      ierr = 1
      return        
20      ierr = 2
      return  
               
      end  
      
      
      subroutine read_int_file_form(ID, form, dat,  m, n, ierr)

      integer dat(*)
      integer m,n
      character form*(*)
      integer ierr
      
      do 72 i=1,m
          read(ID,form,end=10,err=20) (dat(j*m+i),j=0, n-1)
      
 72   continue
      return
      
10      ierr = 1
      return        
20      ierr = 2
      return 
      
      end
      
      subroutine read_int_line_file_form(ID, form, dat, n, ierr)

      integer dat(*)
      integer n
      character form*(*)
      integer ierr

      read(ID,form,end=10,err=20) dat(1:n)
      
      return

10      ierr = 1
      return        
20      ierr = 2
      return 
      
      end