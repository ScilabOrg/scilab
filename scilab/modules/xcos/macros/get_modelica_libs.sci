//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Jérôme PICARD
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function [modelica_libs,mlibsM] = get_modelica_libs()

// Scicos Modelica library path definitions
// modelica_libs contains the Modelica directory paths 

  lhs = argn(1);
  
  // for the standard electrical and hydraulical components 
  modelica_libs = 'SCI/modules/scicos_blocks/macros/' + ['Electrical','Hydraulics'];
  
  // add TMPDIR/modelica for generic modelica blocks
  // needed by modelicat to compile every modelica file
  [status,messages] = mkdir(TMPDIR,'modelica');
  if (status==1 | status==2)  then 
    modelica_libs = [modelica_libs,TMPDIR + '/modelica'];
  else
    messagebox(messages);
    return
  end
  
  if lhs == 2
    // path for generic modelica blocks
    mlibsM = pathconvert(TMPDIR+'/modelica/',%f,%t);
  end
  
endfunction