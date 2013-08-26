// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function u=householder(v,w)
    //Syntax
    //u=householder(v [,w])
    //Description
    //given 2 column vectors v w of same size householder(v,w) returns a unitary
    //column vector u, such that (eye-2*u*u')*v is proportional to w.
    //(eye-2*u*u') is the orthogonal Householder reflexion matrix
    //
    // w default value is eye(v). In this case vector (eye-2*u*u')*v is the
    // vector  eye(v)*(+-norm(v))
    [lhs,rhs]=argn(0)
    if rhs<2 then w=eye(v),end
    a=-sqrt((v'*v)/(w'*w))
    u=v+a*w
    u=u/norm(u)
endfunction
