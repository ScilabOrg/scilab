// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ierr=execstr("[myvar]=getscicosvars(''blocks'')","errcatch"); 
if ierr <> 999 then pause,end