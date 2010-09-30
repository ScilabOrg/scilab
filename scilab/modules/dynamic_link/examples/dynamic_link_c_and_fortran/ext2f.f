c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA/ENPC
c 
c This file is released under the 3-clause BSD license. See COPYING-BSD.

      subroutine ext2f(n,a,b,c)
c     simple example 2 (using sin and cos)
c     -->link('ext2f.o','ext2f');
c     -->a=[1,2,3];b=[4,5,6];n=3;
c     -->c=call('ext2f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
c     -->c=sin(a)+cos(b)

      double precision a(*),b(*),c(*)
      do 1 k=1,n
         c(k)=sin(a(k))+cos(b(k))
 1    continue
      return
      end
