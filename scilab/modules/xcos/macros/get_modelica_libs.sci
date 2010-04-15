// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function modelica_libs = get_modelica_libs()

// Scicos Modelica library path definitions
// modelica_libs contains the Modelica directory paths 

  // for the standard electrical and hydraulical components 
  modelica_libs = 'SCI/modules/scicos_blocks/macros/' + ['Electrical','Hydraulics'];
  
  // add TMPDIR/modelica for generic modelica blocks
  // needed by modelicat to compile every modelica file
  [status,message] = mkdir(TMPDIR,'modelica');
  if (status==1 | status==2)  then 
    modelica_libs = [modelica_libs,TMPDIR + '/modelica'];
  else
    messagebox(message);
    return
  end
  
endfunction