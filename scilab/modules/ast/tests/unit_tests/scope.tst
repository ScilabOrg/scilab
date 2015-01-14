// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

// <-- JVM NOT MANDATORY -->

a=[1 2 3];

function new_var()
    a([]) = [];
    assert_checkequal(a, []);
endfunction

function use_previous()
    a;
    a(2) = 4;
    assert_checkequal(a, [1 4 3]);
endfunction

new_var;
asset_checkequal(a, [1 2 3]);
use_previous;
asset_checkequal(a, [1 2 3]);
