// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function [s]=stdevf(x,fre,o)
    //
    //This function  computes the  standard  deviation  of  the values of  a
    //vector or matrix x, each of them  counted with a frequency signaled by
    //the corresponding values of the integer vector  or matrix fre with the
    //same type of x.
    //
    //For  a  vector or matrix   x,  s=stdevf(x,fre) (or s=stdevf(x,fre,'*')
    //returns in scalar s the  standard deviation of all  the entries of  x,
    //each  value counted   with    the   multiplicity indicated    by   the
    //corresponding value of fre.
    //
    //s=stdevf(x,fre,'r')(or,   equivalently, s=stdevf(x,fre,1)) returns  in
    //each  entry of the row   vector s  the standard
    //deviation of  each    column of  x,    each value  counted   with  the
    //multiplicity indicated by the corresponding value of fre.
    //
    //s=stdevf(x,fre,'c')(or, equivalently, s=stdevf(x,fre,2))  returns   in
    //each entry  of the column vector s  the standard
    //deviation of  each row of x, each  value counted with the multiplicity
    //indicated by the corresponding value of fre.
    //
    //
    rhs=argn(2)
    if rhs<2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"stdevf",2,3)),
    elseif rhs==2 then
        o="*"
    end
    if or(size(x)<>size(fre)) then
        error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: Same dimensions expected.\n"),"stdevf",1,2))
    end
    if x==[] then s=%nan;return,end


    //remove the median
    if o=="*" then
        y=x - meanf(x,fre)
    elseif o=="r"|o==1,
        y=x - ones(size(x,o),1)*meanf(x,fre,o)
    elseif o=="c"|o==2,
        y=x - meanf(x,fre,o)*ones(1,size(x,o))
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),...
        "stdevf",3,"""*"",""r"",""c"",1,2")),
    end
    if size(x,o)==1 then
        s=0*sum((y.^2).*fre,o)
    else
        s=sqrt(sum((y.^2).*fre,o)./(sum(fre,o)-1));
    end
endfunction
