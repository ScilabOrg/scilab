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

function hd5test(hdf5FileToLoad, hdf5FileToSave)

  
  // This will create a scs_m and a codegeneration variable
  import_from_hdf5(hdf5FileToLoad);
  
	disp(scs_m);  
  
  for i=1:size(codegeneration),
  		disp(codegeneration(i) );
  end

  export_to_hdf5(hdf5FileToSave, "scs_m");

endfunction

