c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine dspt(r,s,a,nela,inda,ia, at,iat,indat)  
c transpose a sparse matrix using a distribution count sort.
c*** input
c r,s          number of rows,columns in a.
c a            a one-dimensional array containing the non-zero
c                elements of matrix a,arranged row-wise,but in
c                general not ordered within rows.
c  ma         as in msmgus.
c  ia         ia(i) is address in a of first element of row i of a.
c                ia(number of rows + 1) is number of elements in a,+1.
c*** output
c at           a one-dimensional array containing non-zero elements
c                of matrix a transposed,arranged row-wise,and in
c                order within rows.
c  mat        contains control information and column indices for at,
c               in same format as ma...see msmgus.
c*** working storage.
c  iat        iat(i) is address in at of first element in row i of at.
c               iat(s+1) contains number of elements in at,plus 1.
      double precision a(*), at(nela)
      integer inda(*), ia(*), indat(*), iat(*)
      integer r, s
c
c determine column counts of matrix a(i.e row counts of at)
c  in array iat.
c
      do 10 i=1,s+1
        iat(i) = 0
   10 continue

c     computes number of elements for each column
      do 20 i=1,nela
        k = inda(i+r)
        iat(k) = iat(k) + 1
   20 continue
c
c calculate row pointers of at from column counts obtained above.
c pointer for row i stored in location i+1.
c
      itemp1 = iat(1)
      itemp2 = iat(2)
      iat(2) = 1
      if (s.le.1) go to 40
      do 30 i=2,s
        itemp3 = iat(i+1)
        iat(i+1) = iat(i) + itemp1
        itemp1 = itemp2
        itemp2 = itemp3
   30 continue
c
c calculate column indices(in array indat) and numerical values (in array
c  at) of matrix a-transposed using the list pointers iat(i+1) which
c  always point to the next element to be entered in row i of at.
   40 do 60 i=1,r
        j1 = ia(i)
        j2 = ia(i+1) - 1
        if (j1.gt.j2) go to 60
        do 50 jp=j1,j2
          j = inda(jp + r)
          jpt = iat(j+1)
          indat(jpt + s) = i
          at(jpt) = a(jp)
          iat(j+1) = jpt + 1
   50   continue
   60 continue
c
c now all row pointers iat for at have correct values except position 1.
c  fix it.
      iat(1) = 1
c  extract needed control information in indat.
      do 70 i=1,s
        indat(i) = iat(i+1) - iat(i)
   70 continue
      return
      end
