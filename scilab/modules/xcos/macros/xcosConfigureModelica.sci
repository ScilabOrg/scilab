//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function xcosConfigureModelica()

// We need to define {modelica_libs} variable
// was :
//  [scicos_pal, %scicos_menu, %scicos_short, modelica_libs, scicos_pal_libs,...
//   %scicos_lhb_list, %CmenuTypeOneVector, %scicos_gif,%scicos_contrib, ..
//   %scicos_libs, %scicos_with_grid, %scs_wgrid] = ...
//     initial_scicos_tables();
//
// Extract only needed variable from initial_scicos_tables.sci
//Scicos Modelica library path definitions========================================
  modelica_libs = 'SCI/modules/scicos_blocks/macros/' + ['Electrical','Hydraulics'];
  
  // add TMPDIR/Modelica for generic modelica blocks
  // needed by modelicat to compile every modelica file
  status = mkdir(TMPDIR, 'Modelica');
  if isdir(TMPDIR + '/Modelica') then 
    modelica_libs = [modelica_libs,TMPDIR + '/Modelica'];
  end
  
  name=scs_m.props.title(1);
  if ~validvar(name) then 
    messagebox(msprintf(_("%s is not a valid name, please change the title of the diagram."),name),'error','modal');
    return
  end
  
  name=stripblanks(name)+'_im';
  path=TMPDIR+'/';
  path=pathconvert(stripblanks(path),%t,%t)
  

  mofile=path+name+'.mo';
  xmlfile=path+name+'f_init.xml';
  
  [info,err1]=fileinfo(xmlfile);
  [info,err2]=fileinfo(mofile);
  
  if (err1==0 & err2==0) then,   
    if (newest(xmlfile,mofile)==2) then compile=%t;end;
  else 
    compile=%t;
  end

  compile=%t; // Very conservative

  needcompile=4;

  if compile then 
    %Modelica_Init=%t
// in order to generate *_im.mo -> *_im_f.mo -> *_im.xml 
    [bllst,connectmat,clkconnect,cor,corinv,ok]=c_pass1(scs_m); 
    %Modelica_Init=%f
  end
  [info,err1]=fileinfo(xmlfile);

  // Load TCL tools 
  TCL_EvalFile(SCI+"/modules/tclsci/tcl/sciGUI/sciConfig.tcl")
  TCL_EvalFile(SCI+"/modules/tclsci/tcl/sciGUI/sciGUI.tcl")

  // Initialize TCL GUI
  sciGUI_init();
  
  // Load Modelica configuration GUI
  TCL_EvalFile(SCI+"/modules/scicos/tcl/MIHM.tcl")
  
  if err1==0 then 
    scimihm xmlfile
  end  

  // variables needed by compile_init_modelica
  [modelica_libs,bllst,connectmat,clkconnect,cor,corinv]=resume(modelica_libs,bllst,connectmat,clkconnect,cor,corinv); 
  
endfunction

function  Doubleclick(name,last_name)
 
  //%cpr=tlist(['cpr','corinv'],corinv)

  if last_name<>"" then unhilite_modelica_block(modelica_cind_from_name(last_name));end
  hilite_modelica_block(modelica_cind_from_name(name))

endfunction
