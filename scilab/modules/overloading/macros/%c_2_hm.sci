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

// %c_2_hm
// Comparison between hypermatrix of strings and strings
//
// synopsis
//    text2 > hm_text1
//    hm_text1 : a hypermatrix of strings of total size sz
//    text2 : a matrix of strings of size 1
//
// Returns a hypermatrix of same dimensions as hm_text1
// containing the elementwise comparison of hm_text1 and text2
// using the lexical order

function res = %c_2_hm(text2, hm_text1)
    res = (hm_text1 < text2);
endfunction
