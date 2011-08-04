// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function [vcxprojfullname, vcxprojGUID, typeGUID] = dlwCreateVcxproj(targetName, ..
      filenamesList, vcxprojDirectory, outputDirectory, ..
      additionalIncludeDir, preprocessorsDef, additionalOptionsCC, ..
      additionalDependencies, additionOptionsLD, GUID)

  typeGUID = '8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942'; // C/C++ project
  vcxprojfullname = '';
  vcxprojGUID = GUID;
  
  if isfile(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_template.vcxproj') then
    VCXPROJ = mgetl(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_template.vcxproj');
  else
    VCXPROJ = '';
  end
  
  VCXPROJ = strsubst(VCXPROJ, '11111111-1111-1111-1111-111111111111', GUID);
  VCXPROJ = strsubst(VCXPROJ, '<RootNamespace>vs_2010_template</RootNamespace>', '<RootNamespace>' + targetName + '</RootNamespace>');
  VCXPROJ = strsubst(VCXPROJ, '__TEMPLATE_DLL_EXPORTS__', '__' + convstr(targetName,'u') + '_DLL_EXPORTS__');
  
  VCXPROJ = strsubst(VCXPROJ, '<AdditionalIncludeDirectories>%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>', ..
    '<AdditionalIncludeDirectories>' + strcat(additionalIncludeDir, ';') + ';%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>');
  
  VCXPROJ = strsubst(VCXPROJ, '<AdditionalOptions>%(AdditionalOptions)</AdditionalOptions>', ..
  '<AdditionalOptions>' + additionalOptionsCC + '%(AdditionalOptions)</AdditionalOptions>');
  
  VCXPROJ = strsubst(VCXPROJ, '<AdditionalDependencies>%(AdditionalDependencies)</AdditionalDependencies>', ..
    '<AdditionalDependencies>'  + strcat(additionalDependencies, ';') + ';%(AdditionalDependencies)</AdditionalDependencies>');

  for i = 1:size(filenamesList, '*')
    filenamesList(i) = getrelativefilename(vcxprojDirectory, fullpath(filenamesList(i)));
  end
    
  VCXPROJ = strsubst(VCXPROJ, '<ClCompile Include=""vs_2010_template.cpp"" />', ..
    strcat('<ClCompile Include=""' + filenamesList + '"" />', ascii(10)));
    
  vcxprojfullname = fullpath(vcxprojDirectory + '/' + targetName + '.vcxproj');
  
  if or(VCXPROJ <> '') then
    mputl(VCXPROJ, vcxprojfullname);
    if ilib_verbose() > 1 then
      disp(VCXPROJ);
    end
  else
    // vs_2010_template.vcxproj not found
    warning(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_template.vcxproj'+ _('not found.') );
  end
endfunction
