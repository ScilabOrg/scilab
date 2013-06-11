
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003 - INRIA - Carlos Klimann
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//


function [s]=nancumsum(x,orient)
    //
    //This function returns in scalar or vector s the sum of the
    //values (ignoring the NANs) of  a vector or matrix (real or
    //complex) x.
    //
    //This function  for a vector or a  matrix x, s=nancumsum(x)
    //(or, equivalently  s=nancumsum(x,'*') returns in  scalar s
    //the cumulative sum (ignoring  the NANs) of all the entries
    //of x taken columnwise.
    //
    //s=nancumsum(x,'r')  (or,  equivalently,  s=nancumsum(x,1))
    //returns in  the cols(x) sized vector s  the cumulative sum
    //(ignoring    the    NANs)    of    the    rows    of    x:
    //s(:,i)=nancumsum(x(:,i))
    //
    //s=nancumsum(x,'c')  (or,  equivalently,  s=nancumsum(x,2))
    //returns in  the rows(x) sized vector s  the cumulative sum
    //(ignoring     NANs)    of     the     columns    of     x:
    //s(i,:)=nancumsum(x(i,:))
    //
    //For the  last two cases,  if a row  or column is  in whole
    //composed of NAN, the corresponding place of s will contain
    //a NAN.
    //
    //
    if argn(2)==1 then  orient="*",end
    isn=isnan(x)
    x(isn)=0
    s=cumsum(x,orient)
    s(find(and(isn,orient)))=%nan

endfunction

