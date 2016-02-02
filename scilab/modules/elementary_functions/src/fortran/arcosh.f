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
c
      double precision function arcosh(x)
c!but
c calcule l'arcosinus hyperbolique d'un double precision
c!liste d'appel
c     double precision function arcosh(x)
c     double precision x
c
      double precision x
      if (x.lt.1.0d+0) go to 10
      arcosh = log(x+sqrt(x*x-1.0d+0))
      return
10    arcosh = 0.0d+0
      return
      end
      double precision function arsinh(x)
c!but
c calcule l'arcsinus hyperbolique d'un double precision
c!liste d'appel
c     double precision function arsinh(x)
c     double precision x
c!
      double precision x
c
      arsinh = log(x+sqrt(x*x+1.0d+0))
      return
      end
