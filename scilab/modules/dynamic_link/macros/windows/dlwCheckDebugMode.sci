// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function bOK = dlwCheckDebugMode()
  str = getenv('DEBUG_SCILAB_DYNAMIC_LINK','NO');
  bOK = (str == 'YES');
endfunction
//=============================================================================
