// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=intl(a,b,z0,r,f,ea,er)
    //If f is a complex-valued function, intl(a,b,z0,r,f) computes
    //the integral of f(z)dz along the complex plane curve defined by
    //  z0 + r.exp(%i*t)
    //for a<=t<=b .
    //(part of the circle with center z0 and radius r with phase between a and b)
    //!

    //First compile f if necessary:

    rhs=argn(2)
    if rhs<7 then
        er=%eps;
    end
    if rhs<6 then
        ea=1.d-12;
    end

    if type(f)==11 then
        comp(f)
    end;
    //Define two functions for the real part and
    //imaginary part of f(g(t))*g'(t) where g(t) is a
    //parametrization of the circle.
    deff("y=real1(t)",[
    "z=r*exp(%i*((1-t)*a+t*b))"
    "y=real(f(z+z0)*%i*(b-a)*z)"] )

    deff("y=imag1(t)",[
    "z=r*exp(%i*((1-t)*a+t*b))"
    "y=imag(f(z+z0)*%i*(b-a)*z)"] )

    r=intg(0,1,real1,ea,er)+%i*intg(0,1,imag1,ea,er)
endfunction
