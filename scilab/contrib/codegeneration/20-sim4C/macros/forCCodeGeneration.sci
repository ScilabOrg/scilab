//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function forCCodeGeneration(hdf5FileToLoad, hdf5FileToSave)
  
  // This will create a scs_m and codegeneration variable.
  import_from_hdf5(hdf5FileToLoad);
  
  //load macro's to the scilab environment
  load("SCI/contrib/codegeneration/20-sim4C/macros/ForC_compile.bin");
  load("SCI/contrib/codegeneration/20-sim4C/macros/generateMCF.bin");
  load("SCI/contrib/codegeneration/20-sim4C/macros/generateIvcIpc.bin");
  load("SCI/contrib/codegeneration/20-sim4C/macros/generateMain.bin");
  load("SCI/contrib/codegeneration/20-sim4C/macros/generateModel.bin");
  
  load("SCI/contrib/codegeneration/20-sim4C/macros/utils.bin");
  load("SCI/contrib/codegeneration/20-sim4C/macros/config.bin");
   
  ierr = execstr("[ok,XX,alreadyran,flgcdgen,szclkINTemp,freof] = ForC_compile(scs_m, [], [], %f); ", 'errcatch');
  if ierr <> 0 then
	  [msg, err] = lasterror();
	  disp(msg);
	  deletefile(hdf5FileToSave);
	  return;
  end

  export_to_hdf5(hdf5FileToSave, "XX");

endfunction

