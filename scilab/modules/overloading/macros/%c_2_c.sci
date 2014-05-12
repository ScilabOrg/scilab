// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Samuel Gougeon <sgougeon@free.fr>
// Copyright (C) Scilab Enterprises - 20xx-2014 - Pierre-Aime Agnel
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// =============================================================================

// %c_2_c
// Comparison between two string matrices
// synopsis
//    text1 > text2
//    text1 : a matrix of strings of size n,m or size 1
//    text2 : a matrix of strings of size n,m or size 1
// Returns a matrix of size n,m containing the elementwise
// comparison of text1 and text2 using the lexical order

function yn = %c_2_c(s1, s2)
    yn = (s2 < s1);
endfunction
