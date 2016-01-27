c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c === LICENSE_END ===
c
      subroutine fmc11b(a,n,ir)
c   factorize a matrix given in a
      implicit double precision (a-h,o-z)
      dimension a(*)
      ir=n
      if(n.gt.1)goto100
      if(a(1).gt.0.d0)return
      a(1)=0.d0
      ir=0
      return
  100 continue
      np=n+1
      ii=1
      do 104 i=2,n
      aa=a(ii)
      ni=ii+np-i
      if(aa.gt.0.d0)goto101
      a(ii)=0.d0
      ir=ir-1
      ii=ni+1
      goto104
  101 continue
      ip=ii+1
      ii=ni+1
      jk=ii
      do 103 ij=ip,ni
      v=a(ij)/aa
      do 102 ik=ij,ni
      a(jk)=a(jk)-a(ik)*v
  102 jk=jk+1
  103 a(ij)=v
  104 continue
      if(a(ii).gt.0.d0)return
      a(ii)=0.d0
      ir=ir-1
      return
      end
