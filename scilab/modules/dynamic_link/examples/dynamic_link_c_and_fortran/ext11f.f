
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file is released under the 3-clause BSD license. See COPYING-BSD.

      subroutine ext11f(neq, t, y, ydot)
c     -------------------------------------------
c     same example with call to cmatptr
c     param must be defined as a scilab variable
c     exemple with a call to cmatptr function
c     -->param=[0.04,10000,3d+7];
c     -->link('ext11f.o','ext11f')
c     -->y=ode([1;0;0],0,[0.4,4],'ext11f')
c     Copyright Inria
      double precision t, y, ydot
      logical cmatptr
      dimension y(3), ydot(3)
c
      include 'stack.h'
c

c     If variable 'param' exists gets its pointer in 
c     internal stack else return
      if(.not.cmatptr('param'//char(0),m,n,lp)) return
c     ********************************
c     param entries are in stk(lp),stk(lp+1),stk(lp+2)
c     i.e. param(1)= stk(lp), ...
c     m,n = dimensions of param = 3,1 (or 1,3 if row v.)

c     (note that vector param not used in this example)
      ydot(1) = -stk(lp)*y(1) + stk(lp+1)*y(2)*y(3)
      ydot(3) = stk(lp+2)*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      return
      end
