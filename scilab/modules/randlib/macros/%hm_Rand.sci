// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON (sgougeon@free.fr)
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which you
// should have received as part of this distribution.  The terms are also
// available at http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// Support to hypermatrix generation with grand(HM,...)
// See bug http://bugzilla.scilab.org/5207

function r = %hm_Rand(hm, varargin)
    r = matrix(grand(1, size(hm,"*"), varargin(:)), size(hm))
endfunction
