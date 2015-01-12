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