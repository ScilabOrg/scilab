// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function [vfprojfullname, vfprojGUID, typeGUID] = dlwCreateVfproj(targetName, ..
      filenamesList, vfprojDirectory, outputDirectory, ..
      additionalIncludeDir, preprocessorsDef, additionalOptionsFC, ..
      additionalDependencies, additionOptionsLD, GUID)

  typeGUID = '6989167D-11E4-40FE-8C1A-2192A86A7E90'; // Intel Fortran project
  vfprojfullname = '';
  vfprojGUID = GUID;

  if isfile(SCI+'/modules/dynamic_link/src/scripts/vs_templates/fortran_template.vfproj') then
    VFPROJ = mgetl(SCI+'/modules/dynamic_link/src/scripts/vs_templates/fortran_template.vfproj');
  else
    VFPROJ = '';
  end

  VFPROJ = strsubst(VFPROJ, '11111111-1111-1111-1111-111111111111', GUID);

  for i = 1:size(filenamesList, '*')
    filenamesList(i) = getrelativefilename(vfprojDirectory, fullpath(filenamesList(i)));
  end
  
  STR = strcat("<File RelativePath=""" + filenamesList + """/>", ascii(10)) + "</Filter></Files>";
  VFPROJ = strsubst(VFPROJ, "<File RelativePath="".\fortran_template.f""/></Filter></Files>", STR);

  additionalDependencies = strcat(additionalDependencies, ' ')
  VFPROJ = strsubst(VFPROJ, "$(SCIDIR)/bin/libscilab.lib", ..
                    additionalDependencies);

  VFPROJ = strsubst(VFPROJ, "$(SCIDIR)/modules/core/includes", strcat(additionalIncludeDir,';'));

  vfprojfullname = fullpath(vfprojDirectory + '/' + targetName + '.vfproj');
  
  if or(VFPROJ <> '') then
    mputl(VFPROJ, vfprojfullname);
    if ilib_verbose() > 1 then
      disp(VFPROJ);
    end
  else
    // fortran_template.vcxproj not found
    warning(SCI+'/modules/dynamic_link/src/scripts/vs_templates/fortran_template.vfproj'+ _('not found.') );
  end

endfunction
