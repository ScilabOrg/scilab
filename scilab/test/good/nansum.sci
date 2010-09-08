// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function s=nansum(x,orient)
//
//This function returns in scalar or vector s the sum of the
//values (ignoring the NANs) of a vector or matrix x.
//
//For a vector or matrix  x, s=nansum(x) returns in scalar s
//the sum of all the entries (ignoring the NANs) of x.
//
//s=nansum(x,'r')(or,  equivalently,  s=nansum(x,1)) returns
//in each  entry of the  row vector s of  type 1xsize(x,'c')
//the sum of each column of x (ignoring the NANs).
//
//s=nansum(x,'c')(or,  equivalently,  s=nansum(x,2)) returns
//in each entry of the column vector s of type size(x,'c')x1
//the sum of each row of x (ignoring the NANs).
//
//For the  last two cases,  if a row  or column is  in whole
//composed of NAN, the corresponding place of s will contain
//a NAN.
//
//
//fixed: 2003/09/03
//error texts and all NAN rows or columns
//
  if argn(2)==0 then error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"nansum",1,2)), end
  if argn(2)==1 then  orient='*',end
  isn=isnan(x)
  x(isn)=0
  s=sum(x,orient)
  s(find(and(isn,orient)))=%nan
endfunction
