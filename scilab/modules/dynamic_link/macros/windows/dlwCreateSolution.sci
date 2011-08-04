// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function slnfullname = dlwCreateSolution(solutionName, solutionDirectory, vcxprojfullname, ..
                                         vcxprojGUID, typeGUID)

  if isfile(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_template.sln') then
    SLN = mgetl(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_template.sln');
  else
    SLN = '';
  end

  SLN = strsubst(SLN, '8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942', typeGUID);
  SLN = strsubst(SLN, 'vs_2010_template.vcxproj', getrelativefilename(solutionDirectory,vcxprojfullname));
  SLN = strsubst(SLN, 'vs_2010_template', solutionName);
  SLN = strsubst(SLN, '11111111-1111-1111-1111-111111111111', vcxprojGUID);
  
  slnfullname = fullpath(solutionDirectory + '/' + solutionName + '.sln');
  
  if or(SLN <> '') then
    mputl(SLN, slnfullname);
    if ilib_verbose() > 1 then
      disp(slnfullname);
    end
  else
    // vs_2010_template.vcxproj not found
    warning(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_template.sln'+ _('not found.') );
  end  
endfunction