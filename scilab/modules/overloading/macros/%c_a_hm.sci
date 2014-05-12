// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 20xx-2014 - Pierre-Aime Agnel
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// =============================================================================

// %c_a_hm
// Concatenation of a string to a hypermatrix of strings
//
// synopsis
//    res =  text2 + hm_text1
//    hm_text1 : a hypermatrix of strings of total size sz
//    text2 : a matrix of strings of size 1
//
// Returns a hypermatrix of same dimensions as hm_text1
// containing the concatenation of text2 with each element of hm_text1

function hm_text1 = %c_a_hm(text2, hm_text1)
    if (size(text2, '*') <> 1)
        error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "%c_a_hm", 1, 1, 1));
    end
    hm_text1.entries = text2 + hm_text1.entries;
endfunction


