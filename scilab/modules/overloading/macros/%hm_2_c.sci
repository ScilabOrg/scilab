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

// %hm_2_c
// Comparison between hypermatrix of strings and strings
//
// synopsis
//    hm_text1 > text2
//    hm_text1 : a hypermatrix of strings of total size sz
//    text2 : a matrix of strings of size 1
//
// Returns a hypermatrix of same dimensions as hm_text1
// containing the elementwise comparison of hm_text1 and text2
// using the lexical order

function hm_text1 = %hm_2_c(hm_text1, text2)
    if typeof(hm_text1(1)) <> "string"
        error(msprintf(_("%s: This feature has not been implemented: Comparison between hypermatrices of <%s> and matrices of <%s>"), "%hm_2_c", typeof(hm_text1(1)), typeof(text2)));
    end
    if size(text2, '*') <> 1
        error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "%hm_2_c", 2, 1, 1));
    end
    hm_text1("entries") = (hm_text1("entries") > text2);
endfunction

