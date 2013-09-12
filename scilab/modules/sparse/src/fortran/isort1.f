c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine isort1(count,n,index,flag)
c     
c     !purpose
c     isort sort integer array,maintaining an index array
c     
c     !calling sequence
c     subroutine isort1(count,n,index,flag)
c     integer n,index(n)
c     integer count(n),flag
c     
c     count   : array to be sorted
c     n       :size of count and index
c     index   : array containing on return index of sorted array
c     flag    : 1 : increasing order
c     0 : decreasing order
c     
c     !method
c     quick sort method is used
c     !restriction
c     n must be less than 2**(50/2) ! due to lengh of work space mark
c     !
      dimension mark(50),index(n)
      integer count(n),av,x,flag
c     
c     set index array to original order .
      do 10 i=1,n
         index(i)=i
 10   continue
c     check that a trivial case has not been entered .
      if(n.le.1) goto 300
c     'm' is the length of segment which is short enough to enter
c     the final sorting routine. it may be easily changed.
      m=12
c     set up initial values.
      la=2
      is=1
      if=n
      do 190 mloop=1,n
c     if segment is short enough sort with final sorting routine .
         ifka=if-is
         if((ifka+1).gt.m) goto 70
c*********final sorting ***
c     ( a simple bubble sort )
         is1=is+1
         do 60 j=is1,if
            i=j
 40         if(count(i-1).gt.count(i)) goto 60
            if(count(i-1).lt.count(i)) goto 50
            if(index(i-1).lt.index(i)) goto 60
 50         av=count(i-1)
            count(i-1)=count(i)

            count(i)=av
            int=index(i-1)
            index(i-1)=index(i)
            index(i)=int
            i=i-1
            if(i.gt.is) goto  40
 60      continue
         la=la-2
         goto 170
c     *******  quicksort  ********
c     select the number in the central position in the segment as
c     the test number.replace it with the number from the segment's
c     highest address.
 70      iy=(is+if)/2
         x=count(iy)
         intest=index(iy)
         count(iy)=count(if)
         index(iy)=index(if)
c     the markers 'i' and 'ifk' are used for the beginning and end
c     of the section not so far tested against the present value
c     of x .
         k=1
         ifk=if
c     we alternate between the outer loop that increases i and the
c     inner loop that reduces ifk, moving numbers and indices as
c     necessary, until they meet .
         do 110 i=is,if
            if(x.lt.count(i)) goto 110
            if(x.gt.count(i)) goto 80
            if(intest.gt.index(i)) goto 110
 80         if(i.ge.ifk) goto 120
            count(ifk)=count(i)
            index(ifk)=index(i)
            k1=k
            do 100 k=k1,ifka
               ifk=if-k
               if(count(ifk).lt.x) goto 100
               if(count(ifk).gt.x) goto 90
               if(intest.le.index(ifk)) goto 100
 90            if(i.ge.ifk) goto 130
               count(i)=count(ifk)
               index(i)=index(ifk)
               go to 110
 100        continue
            goto 120
 110     continue
c     return the test number to the position marked by the marker
c     which did not move last. it divides the initial segment into
c     2 parts. any element in the first part is less than or equal
c     to any element in the second part, and they may now be sorted
c     independently .
 120     count(ifk)=x
         index(ifk)=intest
         ip=ifk
         goto 140
 130     count(i)=x
         index(i)=intest
         ip=i
c     store the longer subdivision in workspace.
 140     if((ip-is).gt.(if-ip)) goto 150
         mark(la)=if
         mark(la-1)=ip+1
         if=ip-1
         goto 160
 150     mark(la)=ip-1
         mark(la-1)=is
         is=ip+1
c     find the length of the shorter subdivision.
 160     lngth=if-is
         if(lngth.le.0) goto 180
c     if it contains more than one element supply it with workspace .
         la=la+2
         goto 190
 170     if(la.le.0) goto 200
c     obtain the address of the shortest segment awaiting quicksort
 180     if=mark(la)
         is=mark(la-1)
 190  continue
 200  continue
      if(flag.eq.0) goto 300
      do 201 i=1,n/2
         j=n+1-i
         ii=index(i)
         index(i)=index(j)
         index(j)=ii
         ii=count(i)
         count(i)=count(j)
         count(j)=ii
 201  continue
 300  return
      end
