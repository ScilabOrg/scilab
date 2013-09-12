// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function ww=ascii2string(w1)
    //converts a numeric (real or integer) matrix made of ascii codes
    //into a one column scilab string matrix and conversely.
    ww=[];
    for k=1:size(w1,1)
        ww=[ww;ascii(w1(k,:))];
    end
endfunction
