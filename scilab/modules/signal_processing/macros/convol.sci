// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function [y,e1]=convol(h,x,e0)
    //  convol - convolution
    //%CALLING SEQUENCE
    //  [y]=convol(h,x)
    //  [y,e1]=convol(h,x,e0)     (for use with overlap add method)
    //%PARAMETERS
    //  x,h       :input sequences (h is a "short" sequence, x a "long" one)
    //  e0        : old tail to overlap add (not used in first call)
    //  y         : output of convolution
    //  e1        : new tail to overlap add (not used in last call)
    //%DESCRIPTION
    //  calculates the convolution y= h*x of two discrete sequences by
    //  using the fft.  overlap add method can be used.
    //%USE OF OVERLAP ADD METHOD
    //  For x=[x1,x2,...,xNm1,xN]
    //  First call : [y1,e1]=convol(h,x1)
    //  Subsequent calls : [yk,ek]=convol(h,xk,ekm1)
    //  Final call : [yN]=convol(h,xN,eNm1)
    //  Finally y=[y1,y2,...,yNm1,yN]
    //!

    [lhs,rhs]=argn(0)
    n=prod(size(x))
    m=prod(size(h))
    m1=2^(int(log(n+m-1)/log(2))+1)
    x(m1)=0;h(m1)=0
    if norm(imag(x))==0&norm(imag(h))==0 then
        y=real(fft(fft(matrix(x,1,m1),-1).*fft(matrix(h,1,m1),-1),1))
    else
        y=fft(fft(matrix(x,1,m1),-1).*fft(matrix(h,1,m1),-1),1)
    end
    if lhs+rhs==5 then,
        e0(n)=0;//update carried from left to right
        e1=y(n+1:n+m-1)
        y=y(1:n)+e0

    elseif lhs+rhs==4 then
        if rhs==2 then
            e1=y(n+1:n+m-1)
            y=y(1:n) //initial update
        else
            e0(n+m-1)=0 //final update
            y=y(1:n+m-1)+e0
        end,

    else
        y=y(1:n+m-1) //no update
    end
endfunction
