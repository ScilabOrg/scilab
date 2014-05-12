// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 20xx-2014 - Pierre-Aime Agnel
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// =============================================================================

// %hm_a_c
// Concatenation of a string to a hypermatrix of strings
//
// synopsis
//    res = hm_text1 + text2
//    hm_text1 : a hypermatrix of strings of total size sz
//    text2 : a matrix of strings of size 1
//
// Returns a hypermatrix of same dimensions as hm_text1
// containing the concatenation of each element of hm_text1 with text2

function hm_text1 = %hm_a_c(hm_text1, text2)
    if typeof(hm_text1(1)) <> "string"
        error(msprintf(_("%s: This feature has not been implemented: Addition of hypermatrices of <%s> and matrices of <%s>"), "%hm_a_c", typeof(hm_text1(1)), typeof(text2)));
    end
    if size(text2, '*') <> 1
        error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "%hm_a_c", 2, 1, 1));
    end
    hm_text1.entries = hm_text1.entries + text2;
endfunction

