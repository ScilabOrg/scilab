// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ierr = execstr("a = with_embedded_jre(''TOTO'',2);","errcatch");
if ierr <> 77 then pause,end

result = with_embedded_jre();
if result <> %t & result <> %f then pause, end
