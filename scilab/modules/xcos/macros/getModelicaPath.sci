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
// return the Modelica directory path modelica_path
// using by Xcos diagram containing Modelica blocks

function [modelica_path,modelica_directory] = getModelicaPath()

  modelica_path = [];
  modelica_directory = [];
  
  lhs = argn(1);
  
  // for the standard electrical and hydraulical components 
  modelica_path = 'SCI/modules/scicos_blocks/macros/' + ['Electrical','Hydraulics'];
  
  // add TMPDIR/modelica for generic modelica blocks
  // needed by modelicat to compile every modelica file
  
  [modelica_status,modelica_messages] = mkdir(TMPDIR,'modelica');
  
  if (modelica_status==1 | modelica_status==2)  then 
    modelica_path = [modelica_path,TMPDIR + '/modelica'];
  else
    error(msprintf(gettext("%s \n" ),modelica_messages));
    return
  end
  
  if lhs == 2
    // path for generic modelica blocks
    modelica_directory = pathconvert(TMPDIR+'/modelica/',%f,%t);
  elseif (lhs>2)
    error(msprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "getModelicaPath",1,2));
    return
  end
  
endfunction