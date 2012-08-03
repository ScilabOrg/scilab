// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

if getos() == 'Windows' then
  txt = ['message:',
	    '  @echo ------------------------------------------',
        '  @echo From Makefile.mak',
	    '  @echo ------------------------------------------',
	    ' '];
  mputl(txt,TMPDIR+'/makefile.mak');
  current_dir = pwd();  
  cd TMPDIR;
  files = G_make(['',''],'message');
  cd(current_dir);
end
