// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// Copyright (C) 2012 - Samuel GOUGEON
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//===========================================================
function [y] = toolboxes(path)
// INTERNAL macro should NOT used by users
// toolboxes loading
// path is a directory to explore for contribs
//===========================================================
  global %toolboxes
  global %toolboxes_dir
//===========================================================
  [lhs,rhs] = argn(0) 
  y = [];
  if (rhs == 1) & typeof(path)=="constant" then 
    // return string to exec
    y = 'exec(""" + %toolboxes_dir + %toolboxes(path) + filesep() + "loader.sce" + """);';
    return 
  end

// Non ATOMS modules
  if rhs == 0 then 
    path = SCI + filesep() + 'contrib';
  end
  
  cur_wd = pwd();
  chdir(path);
  files = listfiles('.');
  contribs = [];
  for k = 1:size(files,'*') 
    if isfile(files(k)+'/loader.sce') then 
      contribs = [contribs ; files(k)];
    end
  end

    // ATOMS modules without autoloading
    installed   = atomsGetInstalled()
    autoloading = atomsAutoloadList() 
    for i = 1:size(installed,1)
         if and(installed(i,1)~=autoloading(:,1)) then
              tmpath = installed(i,4)+filesep()+"loader.sce"
              if isfile(tmpath) then 
                   contribs = [contribs ; installed(i,1)+filesep()+installed(i,2)]
              end
         end
    end

  if (contribs <> []) & (getscilabmode() == 'STD') then 
    delmenu(gettext("&Toolboxes"));
    h = uimenu("parent", 0, "label", gettext("&Toolboxes"));
    for k=1:size(contribs,'*')
      m = uimenu(h,'label', string(contribs(k)), 'callback','execstr(toolboxes('+string(k)+'))');
    end
    unsetmenu(gettext("&Toolboxes"));
  end
  
  %toolboxes = contribs;
  %toolboxes_dir = pathconvert(path);
  chdir(cur_wd);
  
endfunction
//===========================================================







